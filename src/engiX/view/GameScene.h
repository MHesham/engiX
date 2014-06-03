#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <d3d11.h>
#include "Timer.h"
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
        ~GameScene();
        void OnUpdate(_In_ const Timer& time);
        void OnRender();
        HRESULT OnConstruct();
        void OnActorCreatedEvt(_In_ EventPtr pEvt);
        void OnActorDestroyedEvt(_In_ EventPtr pEvt);
        void OnToggleCameraEvt(_In_ EventPtr pEvt);
        bool Init();
        std::shared_ptr<SceneCameraNode> Camera();
        void PushTransformation(_In_ const Mat4x4& t);
        void PopTransformation();
        const Mat4x4 TopTransformation() const { return m_worldTransformationStack.top(); }
        std::shared_ptr<SceneCameraNode> AddCamera();

    protected:
        ISceneNode* m_pSceneRoot;
        std::vector<std::shared_ptr<SceneCameraNode>> m_cameras;
        std::stack<Mat4x4> m_worldTransformationStack;
        ID3D11RasterizerState* m_pWireframeRS;
        int m_currCameraIdx;
    };
    
    typedef std::shared_ptr<GameScene> StrongGameScenePtr;
}