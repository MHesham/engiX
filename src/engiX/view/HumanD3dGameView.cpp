#include "HumanD3dGameView.h"
#include <DirectXColors.h>
#include "DXUT.h"
#include "Logger.h"
#include "EventManager.h"
#include "GameScene.h"

using namespace engiX;
using namespace DirectX;


bool HumanD3dGameView::Init()
{
    GameScene::CameraList cameras;

    m_pScene = StrongGameScenePtr(eNEW GameScene);

    cameras.push_back(StrongSceneCameraPtr(eNEW SceneCamera));
    cameras.push_back(StrongSceneCameraPtr(eNEW SceneCamera));

    // A camera perpendicular to the scene
    cameras[0]->PlaceOnSphere(500.0, 0.0, 0.5 * R_PI);
    // A camera with a 45 degree to the scene
    cameras[1]->PlaceOnSphere(500.0, 0.0, R_PI);

    m_pScene->SetCameras(cameras);

    return true;
}

void HumanD3dGameView::OnUpdate(_In_ const Timer& time)
{
    m_pScene->OnUpdate(time);
}

void HumanD3dGameView::OnRender()
{
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    DXUTGetD3D11DeviceContext()->ClearRenderTargetView(pRTV, Colors::LightBlue);
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    DXUTGetD3D11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

    m_pScene->OnRender();
}

bool HumanD3dGameView::OnMsgProc(_In_ const Timer& time, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    switch (uMsg) 
    {
    case WM_KEYDOWN:
        OnKeyDown(time, (BYTE)wParam);
        break;

    case WM_KEYUP:
        OnKeyUp(time, (BYTE)wParam);
        break;
    }

    return true;
}

void HumanD3dGameView::OnKeyDown(_In_ const Timer& time, _In_ const BYTE c)
{
    LogInfo("KeyDown=%c", c);
}

void HumanD3dGameView::OnKeyUp(_In_ const Timer& time, _In_ const BYTE c) 
{ 
    LogInfo("KeyUp=%c", c);

    if (c == 'C')
    {
        LogInfo("Toggle Camera");
        EventPtr toggleCamera(eNEW ToggleCameraEvt(time.TotalTime()));
        g_EventMgr->Queue(toggleCamera);
    }
}


