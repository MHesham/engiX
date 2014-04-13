#include "GameScene.h"
#include "SceneCamera.h"
#include "EventManager.h"

using namespace engiX;

GameScene::GameScene() :
    m_activeCameraIdx(0),
    m_toggleCameraHandler(this, &GameScene::OnToggleCamera)
{
}

bool GameScene::Init()
{
    g_EventMgr->Register(&m_toggleCameraHandler, ToggleCameraEvt::TypeID);

    // Add a default camera that looks in somewhere
    m_cameras.push_back(StrongSceneCameraPtr(eNEW SceneCamera));
  
    return true;
}

void GameScene::OnUpdate(_In_ const Timer& time)
{

}

void GameScene::OnRender()
{

}

void GameScene::OnToggleCamera(EventPtr pEvt)
{
    m_activeCameraIdx = (m_activeCameraIdx + 1) % m_cameras.size();
    LogInfo("Camera Toggled, current camera Id=%x", m_activeCameraIdx);
}

void GameScene::SetCameras(_In_ CameraList& sceneCameras)
{
    if (sceneCameras.empty())
    {
        LogWarning("Can't set an empty list of cameras for a scene, will retain current camera list");
    }

    m_cameras.assign(sceneCameras.begin(), sceneCameras.end());
    m_activeCameraIdx = 0;
}