#pragma once

#include <Windows.h>
#include "Timer.h"
#include <memory>
#include "ViewInterfaces.h"
#include "GameScene.h"

namespace engiX
{
    class GameScene;

    class HumanD3dGameView : public IGameView
    {
    public:
        HumanD3dGameView() :
            m_pScene(nullptr)
        {}

        bool Init();
        void OnRender();
        void OnUpdate(_In_ const Timer& time);
        bool OnMsgProc(_In_ const Timer& time, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

    private:
        void OnKeyDown(_In_ const Timer& time, _In_ const BYTE c);
        void OnKeyUp(_In_ const Timer& time, _In_ const BYTE c);

    protected:
        StrongGameScenePtr m_pScene;
    };
}