#include <Windows.h>
#include <memory>
#include "GameLogic.h"
#include "WinGameApp.h"
#include "HumanD3dGameView.h"
#include "Actor.h"

using namespace engiX;

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
        StrongActorPtr pHeroActor(eNEW Actor(L"Tanker"));


        return pHeroActor;
    }

private:
    ActorID m_heroActor;

};

class BulletGameApp : public WinGameApp
{
public:
    BulletGameApp() {}
    const wchar_t* VGameAppTitle() const { return L"Bullet Game"; }

protected:
    GameLogic* VCreateLogic() const { return eNEW BulletGameLogic; }
    HumanD3dGameView* VCreateStartView() const { return eNEW HumanD3dGameView; }
};

int WINAPI wWinMain(HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPWSTR    lpCmdLine,
                    int       nCmdShow)
{
    return WinGameApp::Main(eNEW BulletGameApp, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
