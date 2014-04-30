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

using namespace engiX;
using namespace std;
using namespace DirectX;

class BulletGameLogic : public GameLogic
{
public:
    BulletGameLogic() {}

    void LoadLevel()
    {
        StrongActorPtr pHeroActor = BuildHero();
        AddActor(pHeroActor);

        AddActor(BuildTerrain());

        m_heroActor = pHeroActor->Id();
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
        StrongActorPtr pHeroActor(eNEW Actor(L"Hero"));
        
        // 1. Build hero visuals
        BoxMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Red.f[0];
        props.Color.y = DirectX::Colors::Red.f[1];
        props.Color.z = DirectX::Colors::Red.f[2];

        props.Width = 1.0;
        props.Depth = 1.0;
        props.Height = 1.0;

        shared_ptr<BoxMeshComponent> pHeroMeshCmpt(eNEW BoxMeshComponent(props));
        pHeroActor->AddComponent(pHeroMeshCmpt);

        shared_ptr<TransformComponent> pHeroTsfmCmpt(eNEW TransformComponent);
        pHeroTsfmCmpt->Position(Vec3(0.0, 0.0, 0.0));
        pHeroActor->AddComponent(pHeroTsfmCmpt);

        shared_ptr<ParticlePhysicsComponent> pHeroPhyCmpt(eNEW ParticlePhysicsComponent);
        pHeroActor->AddComponent(pHeroPhyCmpt);

        pHeroPhyCmpt->InverseMass(1.0/200.0f);
        pHeroPhyCmpt->Velocity(Vec3(0.0f, 30.0f, 40.0f));
        pHeroPhyCmpt->BaseAcceleraiton(Vec3(0.0, -20.0f, 0.0f));

        return pHeroActor;
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
