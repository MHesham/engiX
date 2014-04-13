#include <Windows.h>
#include <memory>
#include "GameLogic.h"
#include "WinGameApp.h"
#include "HumanD3dGameView.h"
#include "Actor.h"
#include "GeometryGenerator.h"
#include "GeneratedMeshComponent.h"

using namespace engiX;
using namespace std;

class BulletGameLogic : public GameLogic
{
public:
    BulletGameLogic() {}

    void VLoadLevel()
    {
        StrongActorPtr pHeroActor = BuildHeroActor();
        AddActor(pHeroActor);

        m_heroActor = pHeroActor->Id();
    }

    StrongActorPtr BuildHeroActor()
    {
        StrongActorPtr pHeroActor(eNEW Actor(L"HeroTanker"));
        
        // 1. Build hero visuals
        GeometryGenerator::MeshData heroMesh;
        m_meshGenerator.CreateBox(50.0, 50.0, 50.0, heroMesh);

        Color3 heroColor;
        heroColor.x = DirectX::Colors::Green.f[0];
        heroColor.y = DirectX::Colors::Green.f[1];
        heroColor.z = DirectX::Colors::Green.f[2];

        StrongActorComponentPtr pHeroMeshCmpt(eNEW GeneratedMeshComponent(heroMesh, heroColor));
        pHeroActor->AddComponent(pHeroMeshCmpt);

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
