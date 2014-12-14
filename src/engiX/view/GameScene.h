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
        bool Init();
        void Camera(ActorID cameraId) { m_cameraId = cameraId; }
        ActorID Camera() const { return m_cameraId; }
        void PushTransformation(_In_ const Mat4x4& t);
        void PopTransformation();
        const Mat4x4 TopTransformation() const { return m_worldTransformationStack.top(); }
        Mat4x4 CameraWorldViewProjMatrix() const;
        RootSceneNode* SceneRoot() { return m_pSceneRoot; }

    protected:
        RootSceneNode* m_pSceneRoot;
        std::stack<Mat4x4> m_worldTransformationStack;
        ActorID m_cameraId;
    };
    
    typedef std::shared_ptr<GameScene> StrongGameScenePtr;
}