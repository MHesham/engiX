#pragma once

#include <wtypes.h>
#include "engiXDefs.h"

namespace engiX
{
    class GameLogic;

    class GameApp
    {
    public:
        class Configurations
        {
        public:
            Configurations() :
                DebugTransform(false)
            {}

            bool DebugTransform;
        };

        GameApp() :
            m_pGameLogic(nullptr)
        {}

        GameLogic* Logic() const { return m_pGameLogic; }
        virtual bool Init(HINSTANCE hInstance, LPWSTR lpCmdLine) = 0;
        virtual void Deinit() = 0;
        virtual void Run() = 0;
        virtual int ExitCode() const = 0;
        virtual real AspectRatio() const = 0;
        const Configurations& Configs() const { return m_configs; }

        GameLogic *m_pGameLogic;
        Configurations m_configs;
    };

    extern GameApp* g_pApp;
}