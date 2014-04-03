#pragma once

#include "WinGameApp.h"

namespace engiX
{
    class BulletGameApp : public WinGameApp
    {
    public:
        BulletGameApp() {}
        const wchar_t* VGameAppTitle() const { return L"Bullet Game"; }
    };
}