#include <Windows.h>
#include <memory>
#include "GameLogic.h"
#include "WinGameApp.h"
#include "HumanD3dGameView.h"
#include "Actor.h"
#include "GeometryGenerator.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
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
        StrongActorPtr pHeroActor = BuildHeroActor();
        AddActor(pHeroActor);

        m_heroActor = pHeroActor->Id();
    }

    StrongActorPtr BuildCameraActor()
    {
        StrongActorPtr pCamActor(eNEW Actor(L"HeroCamera"));

        StrongActorComponentPtr pCamTsfmCmpt(eNEW TransformComponent);
        pCamActor->AddComponent(pCamTsfmCmpt);
    }

    StrongActorPtr BuildHeroActor()
    {
        StrongActorPtr pHeroActor(eNEW Actor(L"HeroTanker"));
        
        // 1. Build hero visuals
        BoxMeshComponent::Properties props;
        props.Color.x = DirectX::Colors::Green.f[0];
        props.Color.y = DirectX::Colors::Green.f[1];
        props.Color.z = DirectX::Colors::Green.f[2];

        props.Width = props.Height = props.Depth = 50.0;

        shared_ptr<BoxMeshComponent> pHeroMeshCmpt(eNEW BoxMeshComponent(props));
        pHeroActor->AddComponent(pHeroMeshCmpt);

        shared_ptr<TransformComponent> pHeroTsfmCmpt(eNEW TransformComponent);
        pHeroTsfmCmpt->Position(Vec3(0.0, 0.0, 0.0));
        pHeroActor->AddComponent(pHeroTsfmCmpt);

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
    const wchar_t* VGameAppTitle() const { return L"Bullet Game"; }

protected:
    GameLogic* VCreateLogicAndStartView() const 
    {
        GameLogic* pLogic = eNEW BulletGameLogic; 
        pLogic->View(shared_ptr<IGameView>(eNEW HumanD3dGameView));

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
