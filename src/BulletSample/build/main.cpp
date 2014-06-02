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
        GridMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Green.f[0];
        props.Color.y = DirectX::Colors::Green.f[1];
        props.Color.z = DirectX::Colors::Green.f[2];

        props.Width = 100.0;
        props.Depth = 100.0;

        shared_ptr<RenderComponent> pTrnMeshCmpt(eNEW GridMeshComponent(props));
        pTrnActor->AddComponent(pTrnMeshCmpt);

        shared_ptr<TransformCmpt> pTrnTsfmCmpt(eNEW TransformCmpt);
        pTrnTsfmCmpt->Position(Vec3(0.0, 0.0, 0.0));
        pTrnActor->AddComponent(pTrnTsfmCmpt);

        m_worldBounds.Radius(50.0f);

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

        shared_ptr<TransformCmpt> pTankTsfm(eNEW TransformCmpt);
        pTankTsfm->Position(Vec3(-10.0f, 2.0f, -10.0));
        pTank->AddComponent(pTankTsfm);

        return pTank;
    }

    void OnUpdate(_In_ const Timer& time)
    {
        GameLogic::OnUpdate(time);

        m_controller.Update(time);

        if (time.TotalTime() - m_lastTargetGenerationTime > 2)
        {
            GenerateTarget();
            m_lastTargetGenerationTime = time.TotalTime();
        }

        if (m_isChargingFirePower && m_firePowerScale < 5.0f)
        {
            m_firePowerScale += m_firePowerScaleVelocity * time.DeltaTime();
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

    
    StrongActorPtr CreateShellBullet(const TransformCmpt& nozzleTsfm)
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

        shared_ptr<TransformCmpt> pBulletTsfm(eNEW TransformCmpt);
        pBulletTsfm->Transform(nozzleTsfm);
        pBullet->AddComponent(pBulletTsfm);

        shared_ptr<ParticlePhysicsCmpt> pBulletPhy(eNEW ParticlePhysicsCmpt);
        pBulletPhy->Mass(1.0);
        pBulletPhy->Velocity(Math::Vec3RotTransform(Vec3(0.0, 10.0, 20.0), nozzleTsfm.Transform()));
        pBulletPhy->BaseAcceleraiton(Math::Vec3RotTransform(Vec3(0.0, -20.0f, 0.0f), nozzleTsfm.Transform()));
        pBulletPhy->LifetimeBound(m_worldBounds);
        pBullet->AddComponent(pBulletPhy);

        return pBullet;
    }

    StrongActorPtr CreatePistolBullet(const TransformCmpt& nozzleTsfm)
    {
        StrongActorPtr pBullet(eNEW Actor(L"PistolBullet"));

        SphereMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Black.f[0];
        props.Color.y = DirectX::Colors::Black.f[1];
        props.Color.z = DirectX::Colors::Black.f[2];

        props.Radius = 0.25;

        shared_ptr<SphereMeshComponent> pBulletMesh(eNEW SphereMeshComponent(props));
        pBullet->AddComponent(pBulletMesh);

        shared_ptr<TransformCmpt> pBulletTsfm(eNEW TransformCmpt);
        pBulletTsfm->Transform(nozzleTsfm);
        pBullet->AddComponent(pBulletTsfm);

        shared_ptr<ParticlePhysicsCmpt> pBulletPhy(eNEW ParticlePhysicsCmpt);
        pBulletPhy->Mass(1.0);
        pBulletPhy->Velocity(Math::Vec3RotTransform(Vec3(0.0, 5.0, 30.0), nozzleTsfm.Transform()));
        pBulletPhy->BaseAcceleraiton(Math::Vec3RotTransform(Vec3(0.0, -5.0f, 0.0f), nozzleTsfm.Transform()));
        pBulletPhy->LifetimeBound(m_worldBounds);
        pBullet->AddComponent(pBulletPhy);

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

        shared_ptr<TransformCmpt> pTargetTsfm(eNEW TransformCmpt);

        real randZ = Math::RandF(15, 45);
        pTargetTsfm->Position(Vec3(0.0, 0.0, randZ));
        pTarget->AddComponent(pTargetTsfm);

        shared_ptr<ParticlePhysicsCmpt> pTargetPhy(eNEW ParticlePhysicsCmpt);
        pTarget->AddComponent(pTargetPhy);

        pTargetPhy->Mass(1.0);
        pTargetPhy->Velocity(Vec3(0.0, 5.0, 0.0));

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
