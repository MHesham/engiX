#pragma once

#include <Windows.h>
#include "Timer.h"
#include <memory>

namespace engiX
{
    class GameScene;

    class HumanD3dGameView
    {
    public:
        HumanD3dGameView();
        void OnRender(_In_ const Timer& time);
        void OnUpdate(_In_ const Timer& time);
        bool OnMsgProc(_In_ const Timer& time, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

    private:
        void OnKeyDown(_In_ const Timer& time, _In_ const BYTE c);
        void OnKeyUp(_In_ const Timer& time, _In_ const BYTE c);

    protected:
        std::shared_ptr<GameScene> m_pScene;
    };
}