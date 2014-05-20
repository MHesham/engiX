#include <Windows.h>
#include <memory>
#include "GameLogic.h"
#include "WinGameApp.h"
#include "HumanD3dGameView.h"
#include "Actor.h"
#include "GeometryGenerator.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ParticlePhysicsComponent.h"
#include <DirectXColors.h>
#include "EventManager.h"
#include "CollisionDetection.h"
#include <algorithm>
#include <set>

using namespace engiX;
using namespace std;
using namespace DirectX;

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
        m_isTurningRight(false),
        m_isTurningLeft(false),
        m_isChargingFirePower(false),
        m_firePowerScaleVelocity(2.0f),
        m_currentWeapon(WPN_Pistol),
        m_turnVelocity(1.5)
    {}

    bool Init()
    {
        CBRB(GameLogic::Init());

        g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &BulletGameLogic::OnStartFirePowerCharge), StartFireWeaponEvt::TypeID);
        g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &BulletGameLogic::OnEndFirePowerCharge), EndFireWeaponEvt::TypeID);
        g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &BulletGameLogic::OnChangeWeapon), ChangeWeaponEvt::TypeID);
        g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &BulletGameLogic::OnStartTurnRight), StartTurnRightEvt::TypeID);
        g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &BulletGameLogic::OnEndTurnRight), EndTurnRightEvt::TypeID);
        g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &BulletGameLogic::OnStartTurnLeft), StartTurnLeftEvt::TypeID);
        g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &BulletGameLogic::OnEndTurnLeft), EndTurnLeftEvt::TypeID);

        return true;
    }

    void LoadLevel()
    {
        StrongActorPtr pHeroTank = CreateHero();
        CBR(AddInitActor(pHeroTank));

        CBR(AddInitActor(CreateTerrain()));

        m_pHero = WeakActorPtr(pHeroTank);
    }

    StrongActorPtr CreateTerrain()
    {
        StrongActorPtr pTrnActor(eNEW Actor(L"Terrain"));

        // 1. Build grid visuals
        GridMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Green.f[0];
        props.Color.y = DirectX::Colors::Green.f[1];
        props.Color.z = DirectX::Colors::Green.f[2];

        props.Width = 100.0;
        props.Depth = 100.0;

        shared_ptr<RenderComponent> pTrnMeshCmpt(eNEW GridMeshComponent(props));
        pTrnActor->AddComponent(pTrnMeshCmpt);

        shared_ptr<TransformComponent> pTrnTsfmCmpt(eNEW TransformComponent);
        pTrnTsfmCmpt->Position(Vec3(0.0, 0.0, 0.0));
        pTrnActor->AddComponent(pTrnTsfmCmpt);

        m_worldBounds.Radius(50.0);

        return pTrnActor;
    }

    StrongActorPtr CreateHero()
    {
        StrongActorPtr pTank(eNEW Actor(L"HeroTank"));

        // 1. Build hero visuals
        BoxMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Blue.f[0];
        props.Color.y = DirectX::Colors::Blue.f[1];
        props.Color.z = DirectX::Colors::Blue.f[2];

        props.Width = 1.0f;
        props.Height = 1.0f;
        props.Depth = 3.0f;

        shared_ptr<BoxMeshComponent> pTankMesh(eNEW BoxMeshComponent(props));
        pTank->AddComponent(pTankMesh);

        shared_ptr<TransformComponent> pTankTsfm(eNEW TransformComponent);
        pTankTsfm->Position(Vec3(-10.0f, 2.0f, -10.0));
        pTank->AddComponent(pTankTsfm);

        return pTank;
    }

    void OnUpdate(_In_ const Timer& time)
    {
        GameLogic::OnUpdate(time);

        CheckActorsLifetime();

        if (time.TotalTime() - m_lastTargetGenerationTime > 2)
        {
            GenerateTarget();
            m_lastTargetGenerationTime = time.TotalTime();
        }

        if (m_isChargingFirePower && m_firePowerScale < 5.0f)
        {
            m_firePowerScale += m_firePowerScaleVelocity * time.DeltaTime();
        }

        if (m_isTurningRight)
        {
            shared_ptr<TransformComponent> pTsfm(m_pHero.lock()->GetComponent<TransformComponent>());
            pTsfm->RotationY(pTsfm->RotationY() + (m_turnVelocity * time.DeltaTime()));
        }

        if (m_isTurningLeft)
        {
            shared_ptr<TransformComponent> pTsfm(m_pHero.lock()->GetComponent<TransformComponent>());
            pTsfm->RotationY(pTsfm->RotationY() - (m_turnVelocity * time.DeltaTime()));
        }
    }

    void CheckActorsLifetime()
    {
        m_deadActors.clear();

        for (auto actorID : m_aliveActors)
        {
            WeakActorPtr pActor = FindActor(actorID);
            if (!pActor.expired())
            {
                shared_ptr<TransformComponent> pTsfm = pActor.lock()->GetComponent<TransformComponent>().lock();

                if (!m_worldBounds.IsPointInside(pTsfm->Position()))
                {
                    RemoveActor(actorID);
                    m_deadActors.insert(actorID);
                }
            }
        }

        if (!m_deadActors.empty())
        {
            for (auto deadActorID : m_deadActors)
                m_aliveActors.erase(deadActorID);
        }
    }

    void GenerateTarget()
    {
        LogInfo("Generating Target");

        StrongActorPtr pTarget(CreateTarget());
        CBR(AddInitActor(pTarget));
        m_aliveActors.insert(pTarget->Id());
    }

    void OnChangeWeapon(EventPtr evt)
    {
        m_currentWeapon = (WeaponType)(((int)m_currentWeapon + (int)WPN_COUNT + 1) % (int)WPN_COUNT);
    }

    void OnStartFirePowerCharge(EventPtr evt)
    {
        _ASSERTE(!m_isChargingFirePower);
        m_isChargingFirePower = true;
        m_firePowerScale = 1.0f;
    }

    void OnEndFirePowerCharge(EventPtr evt)
    {
        StrongActorPtr pBullet;

        if (m_currentWeapon == WPN_Pistol)
            pBullet = CreatePistolBullet();
        else if (m_currentWeapon == WPN_Shell)
            pBullet = CreateShellBullet();

        LogInfo("Firing a bullet with fire power scale %f", m_firePowerScale);

        pBullet->GetComponent<ParticlePhysicsComponent>().lock()->ScaleVelocity(m_firePowerScale);
        pBullet->GetComponent<TransformComponent>().lock()->FrameTransform(
            *m_pHero.lock()->GetComponent<TransformComponent>().lock());

        CBR(AddInitActor(pBullet));
        m_aliveActors.insert(pBullet->Id());
        m_isChargingFirePower = false;
    }

    void OnStartTurnRight(EventPtr evt)
    {
        if (m_pHero.expired())
            return;

        _ASSERTE(!m_isTurningRight);
        m_isTurningRight = true;
    }

    void OnEndTurnRight(EventPtr evt)
    {
        if (m_pHero.expired())
            return;

        _ASSERTE(m_isTurningRight);
        m_isTurningRight = false;
    }

    void OnStartTurnLeft(EventPtr evt)
    {
        if (m_pHero.expired())
            return;

        _ASSERTE(!m_isTurningLeft);
        m_isTurningLeft = true;
    }

    void OnEndTurnLeft(EventPtr evt)
    {
        if (m_pHero.expired())
            return;

        _ASSERTE(m_isTurningLeft);
        m_isTurningLeft = false;
    }

    StrongActorPtr CreateShellBullet()
    {
        StrongActorPtr pBullet(eNEW Actor(L"ShellBullet"));

        BoxMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Brown.f[0];
        props.Color.y = DirectX::Colors::Brown.f[1];
        props.Color.z = DirectX::Colors::Brown.f[2];

        props.Width = 0.5;
        props.Height = 0.5;
        props.Depth = 1.0;

        shared_ptr<BoxMeshComponent> pBulletMesh(eNEW BoxMeshComponent(props));
        pBullet->AddComponent(pBulletMesh);

        shared_ptr<TransformComponent> pBulletTsfm(eNEW TransformComponent);
        pBulletTsfm->Position(Vec3(0.0, 0.0, 0.0));
        pBullet->AddComponent(pBulletTsfm);

        shared_ptr<ParticlePhysicsComponent> pBulletPhy(eNEW ParticlePhysicsComponent);
        pBullet->AddComponent(pBulletPhy);

        pBulletPhy->InverseMass(1.0);
        pBulletPhy->Velocity(Vec3(0.0, 10.0, 20.0));
        pBulletPhy->BaseAcceleraiton(Vec3(0.0, -20.0f, 0.0f));

        return pBullet;
    }

    StrongActorPtr CreatePistolBullet()
    {
        StrongActorPtr pBullet(eNEW Actor(L"PistolBullet"));

        SphereMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Black.f[0];
        props.Color.y = DirectX::Colors::Black.f[1];
        props.Color.z = DirectX::Colors::Black.f[2];

        props.Radius = 0.25;

        shared_ptr<SphereMeshComponent> pBulletMesh(eNEW SphereMeshComponent(props));
        pBullet->AddComponent(pBulletMesh);

        shared_ptr<TransformComponent> pBulletTsfm(eNEW TransformComponent);
        pBulletTsfm->Position(Vec3(0.0, 0.0, 0.0));
        pBullet->AddComponent(pBulletTsfm);

        shared_ptr<ParticlePhysicsComponent> pBulletPhy(eNEW ParticlePhysicsComponent);
        pBullet->AddComponent(pBulletPhy);

        pBulletPhy->InverseMass(1.0);
        pBulletPhy->Velocity(Vec3(0.0, 5.0, 30.0));
        pBulletPhy->BaseAcceleraiton(Vec3(0.0, -5.0f, 0.0f));

        return pBullet;
    }

    StrongActorPtr CreateTarget()
    {
        StrongActorPtr pTarget(eNEW Actor(L"HitTarget"));

        BoxMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Red.f[0];
        props.Color.y = DirectX::Colors::Red.f[1];
        props.Color.z = DirectX::Colors::Red.f[2];

        props.Width = 2.0;
        props.Height = 2.0;
        props.Depth = 0.5;

        shared_ptr<BoxMeshComponent> pTargetMesh(eNEW BoxMeshComponent(props));
        pTarget->AddComponent(pTargetMesh);

        shared_ptr<TransformComponent> pTargetTsfm(eNEW TransformComponent);

        real randZ = MathHelper::RandF(15, 45);
        pTargetTsfm->Position(Vec3(0.0, 0.0, randZ));
        pTarget->AddComponent(pTargetTsfm);

        shared_ptr<ParticlePhysicsComponent> pTargetPhy(eNEW ParticlePhysicsComponent);
        pTarget->AddComponent(pTargetPhy);

        pTargetPhy->InverseMass(1.0);
        pTargetPhy->Velocity(Vec3(0.0, 5.0, 0.0));

        return pTarget;
    }

private:
    WeakActorPtr m_pHero;
    WeakActorComponentPtr m_pHeroTsfm;
    GeometryGenerator m_meshGenerator;
    BoundingSphere m_worldBounds;
    std::set<ActorID> m_aliveActors;
    std::set<ActorID> m_deadActors;
    bool m_isTurningRight;
    bool m_isTurningLeft;
    bool m_isChargingFirePower;
    WeaponType m_currentWeapon;
    real m_lastTargetGenerationTime;
    real m_firePowerScale;
    real m_firePowerScaleVelocity;
    real m_turnVelocity;
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
        pLogic->View(eNEW HumanD3dGameView);

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
