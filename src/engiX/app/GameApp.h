#pragma once

#include <wtypes.h>
#include "engiXDefs.h"

namespace engiX
{
    class GameLogic;

    class GameApp
    {
    public:
        GameApp() :
            m_pGameLogic(nullptr)
        {}

        GameLogic* Logic() const { return m_pGameLogic; }
        virtual bool Init(HINSTANCE hInstance, LPWSTR lpCmdLine) = 0;
        virtual void Deinit() = 0;
        virtual void Run() = 0;
        virtual int ExitCode() const = 0;
        virtual real AspectRatio() const = 0;

        GameLogic *m_pGameLogic;
    };

    extern GameApp* g_pApp;
}