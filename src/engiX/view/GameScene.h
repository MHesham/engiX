#pragma once

#include "Timer.h"
#include <memory>
#include <vector>
#include "Delegate.h"
#include "Events.h"
#include "SceneCamera.h"

namespace engiX
{
    class SceneCamera;

    class GameScene
    {
    public:
        typedef std::vector<StrongSceneCameraPtr> CameraList;

        GameScene();
        void OnUpdate(_In_ const Timer& time);
        void OnRender();
        void OnToggleCamera(EventPtr pEvt);
        bool Init();
        void SetCameras(_In_ CameraList& sceneCameras);

    protected:
        std::vector<StrongSceneCameraPtr> m_cameras;
        size_t m_activeCameraIdx;
        Delegate1P<GameScene, EventPtr> m_toggleCameraHandler;
    };
    
    typedef std::shared_ptr<GameScene> StrongGameScenePtr;
}