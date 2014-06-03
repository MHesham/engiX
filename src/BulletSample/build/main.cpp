#include <Windows.h>
#include <DirectXColors.h>
#include <memory>
#include <algorithm>
#include <set>
#include "GameLogic.h"
#include "WinGameApp.h"
#include "HumanD3dGameView.h"
#include "Actor.h"
#include "GeometryGenerator.h"
#include "RenderComponent.h"
#include "TransformCmpt.h"
#include "ParticlePhysicsCmpt.h"
#include "TurnController.h"
#include "EventManager.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

#define BulletActorName L"Bullet"
#define TargetActorName L"Target"

class BulletGameLogic : public GameLogic
{
public:
    enum WeaponType
    {
        WPN_Pistol,
        WPN_Shell,
        WPN_COUNT
    };

    BulletGameLogic() :
        m_isChargingFirePower(false),
        m_firePowerScaleVelocity(2.0f),
        m_currentWeapon(WPN_Pistol)
    {}

    bool Init()
    {
        CBRB(GameLogic::Init());

        REGISTER_EVT(BulletGameLogic, StartFireWeaponEvt);
        REGISTER_EVT(BulletGameLogic, EndFireWeaponEvt);
        REGISTER_EVT(BulletGameLogic, ChangeWeaponEvt);
        REGISTER_EVT(BulletGameLogic, ActorCreatedEvt);
        REGISTER_EVT(BulletGameLogic, ActorDestroyedEvt);
        REGISTER_EVT(BulletGameLogic, ActorCollisionEvt);

        CBRB(m_controller.Init());

        return true;
    }

    void LoadLevel()
    {
        StrongActorPtr pHeroTank = CreateHero();
        CBR(AddInitActor(pHeroTank));

        m_controller.Control(pHeroTank->Get<TransformCmpt>());

        CBR(AddInitActor(CreateTerrain()));

        m_pHero = WeakActorPtr(pHeroTank);
    }

    StrongActorPtr CreateTerrain()
    {
        StrongActorPtr pTrnActor(eNEW Actor(L"Terrain"));

        // 1. Build grid visuals
        SphereMeshComponent::Properties props;
        props.Color = Color3(DirectX::Colors::Green);
        props.Radius = 50.0;

        pTrnActor->Add<SphereMeshComponent>(props);
        pTrnActor->Add<TransformCmpt>();

        m_worldBounds.Radius(50.0f);

        return pTrnActor;
    }

    StrongActorPtr CreateHero()
    {
        StrongActorPtr pTank(eNEW Actor(L"HeroTank"));

        // 1. Build hero visuals
        BoxMeshComponent::Properties props;
        props.Color = Color3(DirectX::Colors::Blue.f);

        props.Width = 1.0f;
        props.Height = 1.0f;
        props.Depth = 3.0f;

        pTank->Add<BoxMeshComponent>(props);

        shared_ptr<TransformCmpt> pTankTsfm = pTank->Add<TransformCmpt>();

        return pTank;
    }

    void OnUpdate(_In_ const Timer& time)
    {
        GameLogic::OnUpdate(time);

        m_controller.Update(time);

        // Target timed generation logic
        if (time.TotalTime() - m_lastTargetGenerationTime > 0.25f)
        {
            GenerateTarget();
            m_lastTargetGenerationTime = time.TotalTime();
        }

        // Firepower scaling logic
        if (m_isChargingFirePower && m_firePowerScale < 5.0f)
        {
            m_firePowerScale += m_firePowerScaleVelocity * time.DeltaTime();
        }

        CollideActors(time);
    }

    void CollideActors(const Timer& time)
    {
        for (auto bulletId : m_bullets)
        {
            WeakActorPtr pBullet = FindActor(bulletId);
            
            if (pBullet.expired())
                continue;

            for (auto targetId : m_targets)
            {
                WeakActorPtr pTarget = FindActor(targetId);

                if (pTarget.expired())
                    continue;;

                BoundingSphere sphereA = pTarget.lock()->Get<ParticlePhysicsCmpt>().lock()->BoundingMesh();
                BoundingSphere sphereB = pBullet.lock()->Get<ParticlePhysicsCmpt>().lock()->BoundingMesh();

                if (sphereA.Collide(sphereB))
                {
                    g_EventMgr->Queue(EventPtr(eNEW ActorCollisionEvt(time.TotalTime(), bulletId, targetId)));
                }
            }
        }
    }
    
    void GenerateTarget()
    {
        LogInfo("Generating Target");

        StrongActorPtr pTarget(CreateTarget());
        CBR(AddInitActor(pTarget));
    }

    void OnChangeWeaponEvt(EventPtr evt)
    {
        m_currentWeapon = (WeaponType)(((int)m_currentWeapon + (int)WPN_COUNT + 1) % (int)WPN_COUNT);
    }

    void OnStartFireWeaponEvt(EventPtr evt)
    {
        _ASSERTE(!m_isChargingFirePower);
        m_isChargingFirePower = true;
        m_firePowerScale = 1.0f;
    }

    void OnEndFireWeaponEvt(EventPtr evt)
    {
        StrongActorPtr pBullet;

        if (m_currentWeapon == WPN_Pistol)
            pBullet = CreatePistolBullet(*m_pHero.lock()->Get<TransformCmpt>().lock());
        else if (m_currentWeapon == WPN_Shell)
            pBullet = CreateShellBullet(*m_pHero.lock()->Get<TransformCmpt>().lock());

        LogInfo("Firing a bullet with fire power scale %f", m_firePowerScale);

        pBullet->Get<ParticlePhysicsCmpt>().lock()->ScaleVelocity(m_firePowerScale);
        pBullet->Get<TransformCmpt>().lock()->Transform(
            *m_pHero.lock()->Get<TransformCmpt>().lock());

        CBR(AddInitActor(pBullet));
        m_isChargingFirePower = false;
    }

    void OnActorCreatedEvt(EventPtr evt)
    {
        std::shared_ptr<ActorCreatedEvt> pActorEvt = static_pointer_cast<ActorCreatedEvt>(evt);
        StrongActorPtr pActor = FindActor(pActorEvt->ActorId()).lock();

        if (wcscmp(pActor->Typename(), TargetActorName) == 0)
            m_targets.insert(pActorEvt->ActorId());
        else if (wcscmp(pActor->Typename(), BulletActorName) == 0)
            m_bullets.insert(pActorEvt->ActorId());
    }

    void OnActorDestroyedEvt(EventPtr evt)
    {
        std::shared_ptr<ActorCreatedEvt> pActorEvt = static_pointer_cast<ActorCreatedEvt>(evt);
        
        if (!m_targets.erase(pActorEvt->ActorId()))
            m_bullets.erase(pActorEvt->ActorId());
    }

    void OnActorCollisionEvt(EventPtr evt)
    {
        std::shared_ptr<ActorCollisionEvt> pActorEvt = static_pointer_cast<ActorCollisionEvt>(evt);

        RemoveActor(pActorEvt->ActorA());
        RemoveActor(pActorEvt->ActorB());
    }
    
    StrongActorPtr CreateShellBullet(const TransformCmpt& nozzleTsfm)
    {
        StrongActorPtr pBullet(eNEW Actor(BulletActorName));

        BoxMeshComponent::Properties props;
        props.Color = Color3(DirectX::Colors::Brown);

        props.Width = 0.5;
        props.Height = 0.5;
        props.Depth = 1.0;

        pBullet->Add<BoxMeshComponent>(props);
        pBullet->Add<TransformCmpt>()->Transform(nozzleTsfm);

        shared_ptr<ParticlePhysicsCmpt> pBulletPhy = pBullet->Add<ParticlePhysicsCmpt>();
        pBulletPhy->Mass(1.0);
        pBulletPhy->Velocity(Math::Vec3RotTransform(Vec3(0.0, 10.0, 20.0), nozzleTsfm.Transform()));
        pBulletPhy->BaseAcceleraiton(Math::Vec3RotTransform(Vec3(0.0, -20.0f, 0.0f), nozzleTsfm.Transform()));
        pBulletPhy->LifetimeBound(m_worldBounds);
        pBulletPhy->Radius(1.0);

        return pBullet;
    }

    StrongActorPtr CreatePistolBullet(const TransformCmpt& nozzleTsfm)
    {
        StrongActorPtr pBullet(eNEW Actor(BulletActorName));

        SphereMeshComponent::Properties props;
        props.Color = Color3(DirectX::Colors::Black);
        props.Radius = 0.25;

        pBullet->Add<SphereMeshComponent>(props);

        pBullet->Add<TransformCmpt>()->Transform(nozzleTsfm);

        shared_ptr<ParticlePhysicsCmpt> pBulletPhy = pBullet->Add<ParticlePhysicsCmpt>();
        pBulletPhy->Mass(1.0);
        pBulletPhy->Velocity(Math::Vec3RotTransform(Vec3(0.0, 5.0, 30.0), nozzleTsfm.Transform()));
        pBulletPhy->BaseAcceleraiton(Math::Vec3RotTransform(Vec3(0.0, -5.0f, 0.0f), nozzleTsfm.Transform()));
        pBulletPhy->LifetimeBound(m_worldBounds);
        pBulletPhy->Radius(0.25);

        return pBullet;
    }

    StrongActorPtr CreateTarget()
    {
        StrongActorPtr pTarget(eNEW Actor(TargetActorName));

        BoxMeshComponent::Properties props;
        props.Color = Color3(DirectX::Colors::Red);
        props.Width = 2.0;
        props.Height = 2.0;
        props.Depth = 0.5;

        pTarget->Add<BoxMeshComponent>(props);

        real randZ = Math::RandF(15, 45);
        pTarget->Add<TransformCmpt>()->Position(Vec3(0.0, 0.0, randZ));

        shared_ptr<ParticlePhysicsCmpt> pTargetPhy = pTarget->Add<ParticlePhysicsCmpt>();
        pTargetPhy->Mass(1.0);
        pTargetPhy->Velocity(Vec3(0.0, 7.0, 0.0));
        pTargetPhy->Radius(2.0);
        pTargetPhy->LifetimeBound(m_worldBounds);

        return pTarget;
    }

private:
    WeakActorPtr m_pHero;
    WeakActorComponentPtr m_pHeroTsfm;
    GeometryGenerator m_meshGenerator;
    bool m_isChargingFirePower;
    WeaponType m_currentWeapon;
    real m_lastTargetGenerationTime;
    real m_firePowerScale;
    real m_firePowerScaleVelocity;
    TurnController m_controller;
    BoundingSphere m_worldBounds;
    std::set<ActorID> m_bullets;
    std::set<ActorID> m_targets;
};

class BulletD3dGameView : public HumanD3dGameView
{
public:
    bool Init()
    {
        CBRB(HumanD3dGameView::Init());

        m_pScene->AddCamera()->PlaceOnSphere(25.0, 1.60f * R_PI, 0.45f * R_PI);
        m_pScene->AddCamera()->PlaceOnSphere(25.0, 0.25f * R_PI, 0.25f * R_PI);
        m_pScene->AddCamera()->PlaceOnSphere(150.0, 0.5f * R_PI, 0.01f * R_PI);
        m_pScene->AddCamera()->PlaceOnSphere(150.0, 0.0f, 0.48f * R_PI);

        return true;
    }
};

class BulletGameApp : public WinGameApp
{
public:
    BulletGameApp() {}
    const wchar_t* GameAppTitle() const { return L"Bullet Game"; }

protected:
    GameLogic* CreateLogicAndStartView() const 
    {
        GameLogic* pLogic = eNEW BulletGameLogic; 
        pLogic->View(eNEW BulletD3dGameView);

        return pLogic;
    }
};

int WINAPI wWinMain(HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPWSTR    lpCmdLine,
                    int       nCmdShow)
{
    return WinGameApp::Main(eNEW BulletGameApp, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
