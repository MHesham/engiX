#pragma once

#include "Timer.h"
#include <memory>
#include <vector>
#include "Delegate.h"
#include "Events.h"
#include "SceneCameraNode.h"
#include "ViewInterfaces.h"

namespace engiX
{
    class SceneCameraNode;

    class GameScene
    {
    public:
        GameScene();
        void OnUpdate(_In_ const Timer& time);
        void OnRender();
        void OnConstruct();
        void OnToggleCamera(EventPtr pEvt);
        bool Init();

    protected:
        std::shared_ptr<ISceneNode> m_pSceneRoot;
        std::shared_ptr<SceneCameraNode> m_pCameraNode;
    };
    
    typedef std::shared_ptr<GameScene> StrongGameScenePtr;
}