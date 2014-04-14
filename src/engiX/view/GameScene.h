#pragma once

#include "Timer.h"
#include <memory>
#include <vector>
#include "Delegate.h"
#include "Events.h"
#include "SceneCameraNode.h"
#include "ViewInterfaces.h"
#include <stack>

namespace engiX
{
    class SceneCameraNode;

    class GameScene
    {
    public:
        GameScene();
        void OnUpdate(_In_ const Timer& time);
        void OnRender();
        HRESULT OnConstruct();
        void OnToggleCamera(_In_ EventPtr pEvt);
        bool Init();

        void PushTransformation(_In_ const Mat4x4& t);
        void PopTransformation() { m_worldTransformationStack.pop(); }

    protected:
        std::shared_ptr<ISceneNode> m_pSceneRoot;
        std::shared_ptr<SceneCameraNode> m_pCameraNode;
        std::stack<Mat4x4> m_worldTransformationStack;

    };
    
    typedef std::shared_ptr<GameScene> StrongGameScenePtr;
}