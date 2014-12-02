#include "HumanD3dGameView.h"
#include <DirectXColors.h>
#include <algorithm>
#include "DXUT.h"
#include "Logger.h"
#include "EventManager.h"
#include "GameScene.h"

using namespace engiX;
using namespace DirectX;
using namespace std;

bool HumanD3dGameView::Init()
{
    m_pScene = StrongGameScenePtr(eNEW GameScene);
    CBRB(m_pScene->Init());
    fill(begin(m_downKeys), end(m_downKeys), false);
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
    if (m_downKeys[c])
        return;

    LogVerbose("KeyDown=%c", c);

    if (c == 'A')
        g_EventMgr->Queue(EventPtr(eNEW StartTurnLeftEvt(time.TotalTime())));
    else if (c == 'D')
        g_EventMgr->Queue(EventPtr((eNEW StartTurnRightEvt(time.TotalTime()))));
    else if (c == 'W')
        g_EventMgr->Queue(EventPtr(eNEW StartForwardThrustEvt(time.TotalTime())));
    else if (c == 'S')
        g_EventMgr->Queue(EventPtr(eNEW StartBackwardThrustEvt(time.TotalTime())));
    else if (c == ' ')
        g_EventMgr->Queue(EventPtr(eNEW StartFireWeaponEvt(time.TotalTime())));

    m_downKeys[c] = true;
}

void HumanD3dGameView::OnKeyUp(_In_ const Timer& time, _In_ const BYTE c) 
{ 
    LogVerbose("KeyUp=%c", c);

    if (c == 'C')
        g_EventMgr->Queue(EventPtr(eNEW ToggleCameraEvt(time.TotalTime())));
    else if (c == 'A')
        g_EventMgr->Queue(EventPtr(eNEW EndTurnLeftEvt(time.TotalTime())));
    else if (c == 'D')
        g_EventMgr->Queue(EventPtr((eNEW EndTurnRightEvt(time.TotalTime()))));
    else if (c == 'W')
        g_EventMgr->Queue(EventPtr(eNEW EndForwardThrustEvt(time.TotalTime())));
    else if (c == 'S')
        g_EventMgr->Queue(EventPtr(eNEW EndBackwardThrustEvt(time.TotalTime())));
    else if (c == ' ')
        g_EventMgr->Queue(EventPtr(eNEW EndFireWeaponEvt(time.TotalTime())));
    else if (c == 'Q')
        g_EventMgr->Queue(EventPtr(eNEW ChangeWeaponEvt(time.TotalTime())));

    m_downKeys[c] = false;
}


