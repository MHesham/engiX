#pragma once

#include <Windows.h>
#include "Timer.h"

namespace engiX
{
    class IGameView
    {
    public:
        virtual ~IGameView() {}
        virtual bool Init() = 0;
        virtual void OnRender() = 0;
        virtual void OnUpdate(_In_ const Timer& time) = 0;
        virtual bool OnMsgProc(_In_ const Timer& time, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) = 0;
    };

    class ISceneNode
    {
    public:
        virtual ~ISceneNode() {}
        virtual bool PreRender() = 0;
        virtual void PostRender() = 0;
        virtual void OnRender() = 0;
        virtual void OnUpdate(_In_ const Timer& time) = 0;
    };

}