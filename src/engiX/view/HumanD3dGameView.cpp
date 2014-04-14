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
    m_pScene = StrongGameScenePtr(eNEW GameScene);

    return true;
}

HRESULT HumanD3dGameView::OnConstruct()
{
    return m_pScene->OnConstruct();
}

void HumanD3dGameView::OnUpdate(_In_ const Timer& time)
{
    m_pScene->OnUpdate(time);
}

void HumanD3dGameView::OnRender()
{
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


