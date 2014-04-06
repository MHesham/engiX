#include "HumanD3dGameView.h"
#include <DirectXColors.h>
#include "DXUT.h"
#include "Logger.h"
#include "EventManager.h"
#include "GameScene.h"

using namespace engiX;
using namespace DirectX;

HumanD3dGameView::HumanD3dGameView() :
    m_pScene(eNEW GameScene)
{

}

void HumanD3dGameView::OnUpdate(_In_ const Timer& time)
{
    m_pScene->OnUpdate(time);
}

void HumanD3dGameView::OnRender(_In_ const Timer& time)
{
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    DXUTGetD3D11DeviceContext()->ClearRenderTargetView(pRTV, Colors::LightBlue);
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    DXUTGetD3D11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

    m_pScene->OnRender(time);
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

