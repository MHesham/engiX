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

using namespace engiX;
using namespace std;
using namespace DirectX;

class BulletGameLogic : public GameLogic
{
public:
    BulletGameLogic() {}

    bool Init()
    {
        CBRB(GameLogic::Init());

        g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &BulletGameLogic::OnFireWeapon), FireWeaponEvt::TypeID);

        return true;
    }

    void LoadLevel()
    {
        StrongActorPtr pHeroTank = BuildHero();
        CBR(AddInitActor(pHeroTank));

        CBR(AddInitActor(BuildTerrain()));

        m_heroActor = pHeroTank->Id();
    }

    StrongActorPtr BuildTerrain()
    {
        StrongActorPtr pTrnActor(eNEW Actor(L"Terrain"));

        // 1. Build grid visuals
        GridMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Brown.f[0];
        props.Color.y = DirectX::Colors::Brown.f[1];
        props.Color.z = DirectX::Colors::Brown.f[2];

        props.Width = 50.0;
        props.Depth = 50.0;

        shared_ptr<RenderComponent> pTrnMeshCmpt(eNEW GridMeshComponent(props));
        pTrnActor->AddComponent(pTrnMeshCmpt);

        shared_ptr<TransformComponent> pTrnTsfmCmpt(eNEW TransformComponent);
        pTrnTsfmCmpt->Position(Vec3(0.0, 0.0, 0.0));
        pTrnActor->AddComponent(pTrnTsfmCmpt);

        return pTrnActor;
    }

    StrongActorPtr BuildHero()
    {
        StrongActorPtr pTank(eNEW Actor(L"HeroTank"));
        
        // 1. Build hero visuals
        BoxMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Red.f[0];
        props.Color.y = DirectX::Colors::Red.f[1];
        props.Color.z = DirectX::Colors::Red.f[2];

        props.Width = 1.0;
        props.Depth = 4.0;
        props.Height = 1.0;

        shared_ptr<BoxMeshComponent> pTankMesh(eNEW BoxMeshComponent(props));
        pTank->AddComponent(pTankMesh);

        shared_ptr<TransformComponent> pTankTsfm(eNEW TransformComponent);
        pTankTsfm->Position(Vec3(0.0, 0.0, 0.0));
        pTank->AddComponent(pTankTsfm);

        return pTank;
    }

    void OnUpdate(_In_ const Timer& time)
    {
        GameLogic::OnUpdate(time);
    }

    void OnFireWeapon(EventPtr evt)
    {
        StrongActorPtr pBullet = CreateBullet();
        CBR(AddInitActor(pBullet));
    }

    StrongActorPtr CreateBullet()
    {
        StrongActorPtr pBullet(eNEW Actor(L"Bullet"));

        BoxMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Brown.f[0];
        props.Color.y = DirectX::Colors::Brown.f[1];
        props.Color.z = DirectX::Colors::Brown.f[2];

        props.Width = 1.0;
        props.Depth = 1.0;
        props.Height = 1.0;

        shared_ptr<BoxMeshComponent> pBulletMesh(eNEW BoxMeshComponent(props));
        pBullet->AddComponent(pBulletMesh);

        shared_ptr<TransformComponent> pBulletTsfm(eNEW TransformComponent);
        pBulletTsfm->Position(Vec3(0.0, 0.0, 0.0));
        pBullet->AddComponent(pBulletTsfm);

        shared_ptr<ParticlePhysicsComponent> pBulletPhy(eNEW ParticlePhysicsComponent);
        pBullet->AddComponent(pBulletPhy);

        pBulletPhy->InverseMass(1.0/200.0f);
        pBulletPhy->Velocity(Vec3(0.0, 20.0, 30.0));
        pBulletPhy->BaseAcceleraiton(Vec3(0.0, -20.0f, 0.0f));

        return pBullet;
    }

private:
    ActorID m_heroActor;
    GeometryGenerator m_meshGenerator;
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
