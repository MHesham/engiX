#pragma once

#include "Timer.h"
#include <memory>

namespace engiX
{
    class SceneCamera;

    class GameScene
    {
    public:
        GameScene();
        void OnUpdate(_In_ const Timer& time);
        void OnRender(_In_ const Timer& time);

    protected:
        std::shared_ptr<SceneCamera> m_pCamera;
    };
}