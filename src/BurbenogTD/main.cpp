#include "stdafx.h"
#include <DirectXColors.h>
#include "GameLogic.h"
#include "WinGameApp.h"
#include "HumanD3dGameView.h"
#include "Actor.h"
#include "GeometryGenerator.h"
#include "RenderCmpt.h"
#include "TransformCmpt.h"
#include "ParticlePhysicsCmpt.h"
#include "TurnController.h"
#include "EventManager.h"
#include "ActorTurnTask.h"
#include "ParticleForceGen.h"
#include "CameraCmpt.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

#define BulletActorName L"Bullet"
#define TargetActorName L"Target"
#define HeroActorName L"Hero"
#define HeroCameraName L"HeroCam"

class BurbenogLogic : public GameLogic
{
public:
    enum WeaponType
    {
        WPN_Pistol,
        WPN_Shell,
        WPN_COUNT
    };

    BurbenogLogic() :
        m_isChargingFirePower(false),
        m_firePowerScaleVelocity(2.0f),
        m_currentWeapon(WPN_Pistol),
        m_heroId(NullActorID)
    {}

    bool Init()
    {
        CBRB(GameLogic::Init());

        REGISTER_EVT(BurbenogLogic, StartFireWeaponEvt);
        REGISTER_EVT(BurbenogLogic, EndFireWeaponEvt);
        REGISTER_EVT(BurbenogLogic, ChangeWeaponEvt);
        REGISTER_EVT(BurbenogLogic, ActorCreatedEvt);
        REGISTER_EVT(BurbenogLogic, ActorDestroyedEvt);
        REGISTER_EVT(BurbenogLogic, ActorCollisionEvt);

        CBRB(m_controller.Init());

        return true;
    }

    bool LoadLevel()
    {
        ActorUniquePtr pHeroTank(CreateHero());
        m_controller.Control(pHeroTank->Id());
        m_heroId = pHeroTank->Id();

        CBRB(AddInitActor(std::move(pHeroTank)));
        CBRB(AddInitActor(CreateTerrain()));
        CBRB(AddInitActor(CreateWorldBounds()));

        return true;
    }

    ActorUniquePtr CreateWorldBounds()
    {
        ActorUniquePtr pActor(eNEW Actor(L"WorldBounds"));

        // 1. Build grid visuals
        SphereMeshCmpt::Properties props;
        props.Color = Color3(DirectX::Colors::Green);
        props.Radius = 50.0;
        props.IsBackfacing = true;

        pActor->Add<SphereMeshCmpt>(props);
        pActor->Add<TransformCmpt>();

        m_worldBounds.Radius(50.0f);

        m_taskMgr.AttachTask(
            StrongTaskPtr(eNEW ActorTurnTask(pActor->Id(), Vec3(0.0, -0.10f, 0.0))));

        m_worldPullForceId = ForceRegistry().RegisterGenerator(
            std::shared_ptr<ParticleAnchoredSpring>(
            eNEW ParticleAnchoredSpring(Vec3(0.0, 60.0, 0.0), 55, 0.5)));

        return pActor;
    }

    ActorUniquePtr CreateTerrain()
    {
        ActorUniquePtr pActor(eNEW Actor(L"Terrain"));

        // 1. Build grid visuals
        CylinderMeshCmpt::Properties props;
        props.Color = Color3(DirectX::Colors::Brown);
        props.TopRadius = 35.0;
        props.BottomRadius = 15.0;
        props.Height = 20.0;
        props.StackCount = 2;
        props.SliceCount = 20;

        pActor->Add<CylinderMeshCmpt>(props);
        pActor->Add<TransformCmpt>().Position(Vec3(0.0, -30.0, 0.0));

        m_taskMgr.AttachTask(
            StrongTaskPtr(eNEW ActorTurnTask(pActor->Id(), Vec3(0.0, 0.40f, 0.0))));

        return pActor;
    }

    ActorUniquePtr CreateHero()
    {
        ActorUniquePtr pTank(eNEW Actor(L"Hero"));

        // 1. Build hero visuals
        BoxMeshCmpt::Properties props;
        props.Color = Color3(DirectX::Colors::Blue.f);

        props.Width = 1.0f;
        props.Height = 1.0f;
        props.Depth = 4.0f;

        pTank->Add<BoxMeshCmpt>(props);

        pTank->Add<TransformCmpt>().Position(Vec3(0.0, 0.0, -10));

        return pTank;
    }

    void OnUpdate(_In_ const Timer& time)
    {
        GameLogic::OnUpdate(time);

        m_controller.Update(time);

        // Target timed generation logic
        if (time.TotalTime() - m_lastTargetGenerationTime > 0.25f)
        {
            //GenerateTarget();
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
            auto& b = GetActor(bulletId);
            
            if (b.IsNull())
                continue;

            for (auto targetId : m_targets)
            {
                auto& t = GetActor(targetId);

                if (t.IsNull())
                    continue;;

                BoundingSphere sphereA = t.Get<ParticlePhysicsCmpt>().BoundingMesh();
                BoundingSphere sphereB = b.Get<ParticlePhysicsCmpt>().BoundingMesh();

                if (sphereA.Collide(sphereB))
                {
                    g_EventMgr->Queue(EventPtr(eNEW ActorCollisionEvt(time.TotalTime(), bulletId, targetId)));
                }
            }
        }
    }
    
    void GenerateTarget()
    {
        LogVerbose("Generating Target");

        CBR(AddInitActor(CreateTarget()));
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
        ActorUniquePtr pBullet;

        auto& a= g_pApp->Logic()->GetActor(m_heroId);

        if (m_currentWeapon == WPN_Pistol)
            pBullet = CreatePistolBullet(a.Get<TransformCmpt>());
        else if (m_currentWeapon == WPN_Shell)
            pBullet = CreateShellBullet(a.Get<TransformCmpt>());

        LogVerbose("Firing a bullet with fire power scale %f", m_firePowerScale);

        pBullet->Get<ParticlePhysicsCmpt>().ScaleVelocity(m_firePowerScale);
        pBullet->Get<TransformCmpt>().Transform(a.Get<TransformCmpt>().Transform());

        CBR(AddInitActor(std::move(pBullet)));
        m_isChargingFirePower = false;
    }

    void OnActorCreatedEvt(EventPtr evt)
    {
        std::shared_ptr<ActorCreatedEvt> pActorEvt = static_pointer_cast<ActorCreatedEvt>(evt);
        auto& a = GetActor(pActorEvt->ActorId());

        if (wcscmp(a.Typename(), TargetActorName) == 0)
            m_targets.insert(pActorEvt->ActorId());
        else if (wcscmp(a.Typename(), BulletActorName) == 0)
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
    
    ActorUniquePtr CreateShellBullet(const TransformCmpt& nozzleTsfm)
    {
        ActorUniquePtr pBullet(eNEW Actor(BulletActorName));

        BoxMeshCmpt::Properties props;
        props.Color = Color3(DirectX::Colors::Brown);

        props.Width = 0.5;
        props.Height = 0.5;
        props.Depth = 1.0;

        pBullet->Add<BoxMeshCmpt>(props);
        pBullet->Add<TransformCmpt>().Transform(nozzleTsfm.Transform());

        ParticlePhysicsCmpt& pBulletPhy = pBullet->Add<ParticlePhysicsCmpt>();
        pBulletPhy.Mass(1.0);
        pBulletPhy.Velocity(Math::Vec3RotTransform(Vec3(0.0, 10.0, 20.0), nozzleTsfm.Transform()));
        pBulletPhy.BaseAcceleraiton(Math::Vec3RotTransform(Vec3(0.0, -20.0f, 0.0f), nozzleTsfm.Transform()));
        pBulletPhy.LifetimeBound(m_worldBounds);
        pBulletPhy.Radius(1.0);

        return pBullet;
    }

    ActorUniquePtr CreatePistolBullet(const TransformCmpt& nozzleTsfm)
    {
        ActorUniquePtr pBullet(eNEW Actor(BulletActorName));

        SphereMeshCmpt::Properties props;
        props.Color = Color3(DirectX::Colors::Black);
        props.Radius = 0.25;

        pBullet->Add<SphereMeshCmpt>(props);

        pBullet->Add<TransformCmpt>().Transform(nozzleTsfm.Transform());

        ParticlePhysicsCmpt& pBulletPhy = pBullet->Add<ParticlePhysicsCmpt>();
        pBulletPhy.Mass(1.0);
        pBulletPhy.Velocity(Math::Vec3RotTransform(Vec3(0.0, 5.0, 30.0), nozzleTsfm.Transform()));
        pBulletPhy.BaseAcceleraiton(Math::Vec3RotTransform(Vec3(0.0, -5.0f, 0.0f), nozzleTsfm.Transform()));
        pBulletPhy.LifetimeBound(m_worldBounds);
        pBulletPhy.Radius(0.25);

        return pBullet;
    }

    ActorUniquePtr CreateTarget()
    {
        ActorUniquePtr pTarget(eNEW Actor(TargetActorName));

        BoxMeshCmpt::Properties props;
        props.Color = Color3(DirectX::Colors::Red);
        props.Width = 2.0;
        props.Height = 2.0;
        props.Depth = 0.5;

        pTarget->Add<BoxMeshCmpt>(props);

        real randZ = Math::RandF(15, 45);
        real randX = Math::RandF(-45, 45);
        pTarget->Add<TransformCmpt>().Position(Vec3(randX, 0.0, randZ));

        ParticlePhysicsCmpt& pTargetPhy = pTarget->Add<ParticlePhysicsCmpt>();
        pTargetPhy.Mass(1.0);
        //pTargetPhy->Velocity(Vec3(0.0, Math::RandF(7, 15), 0.0));
        pTargetPhy.Radius(2.0);
        pTargetPhy.LifetimeBound(m_worldBounds);

        ForceRegistry().RegisterActorForce(pTarget->Id(), m_worldPullForceId);

        return pTarget;
    }

private:
    ActorID m_heroId;
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
    ParticleForceGenID m_worldPullForceId;
};

class BurbenogTDView : public HumanD3dGameView
{
public:

    bool Init()
    {
        CBRB(HumanD3dGameView::Init());

        ActorUniquePtr pCam(CreateCamera());
        ActorID camId = pCam->Id();
        CBRB(g_pApp->Logic()->AddInitActor(std::move(pCam)));

        m_pScene->Camera(camId);

        return true;
    }

private:
    ActorUniquePtr CreateCamera()
    {
        auto& a = g_pApp->Logic()->GetActor(HeroActorName);
        _ASSERTE(!a.IsNull());

        ActorUniquePtr pHeroCam(eNEW Actor(HeroCameraName));

        auto& camTsfm = pHeroCam->Add<TransformCmpt>();
        //camTsfm.PlaceOnSphere(1200.0, 1.5f * R_PI, 0.45f * R_PI);
        Vec3 pos(0.0f, 50.0f, 50.0f);
        camTsfm.Position(pos);
        camTsfm.LookAt(Vec3(g_XMZero));

        pHeroCam->Add<CameraCmpt>();// .SetAsThirdPerson(a.Id());

        return pHeroCam;
    }
};

class BurbenogTD : public WinGameApp
{
public:
    BurbenogTD() {}
    const wchar_t* GameAppTitle() const { return L"Burbenog TD"; }

protected:
    GameLogic* CreateLogicAndStartView() const 
    {
        GameLogic* pLogic = eNEW BurbenogLogic; 
        pLogic->View(eNEW BurbenogTDView);

        return pLogic;
    }
};

int WINAPI wWinMain(HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPWSTR    lpCmdLine,
                    int       nCmdShow)
{
    return WinGameApp::Main(eNEW BurbenogTD, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
