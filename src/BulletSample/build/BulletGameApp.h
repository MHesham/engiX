#pragma once

#include "WinGameApp.h"
#include "GameLogic.h"

namespace engiX
{
    class BulletGameLogic : public GameLogic
    {

    };

    class BulletGameApp : public WinGameApp
    {
    public:
        BulletGameApp() {}
        const wchar_t* VGameAppTitle() const { return L"Bullet Game"; }
        
    protected:
        GameLogic* VCreateLogic() const { return new BulletGameLogic; }
        GameView* VCreateStartView() const { return nullptr; }
    };
}