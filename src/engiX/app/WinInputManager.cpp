#include "WinInputManager.h"
#include <algorithm>
#include "Logger.h"
#include "EventManager.h"

using namespace std;
using namespace engiX;

WinInputManager::WinInputManager()
{
}

bool WinInputManager::OnMsgProc(_In_ const Timer& time, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
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

bool WinInputManager::OnKeyDown(_In_ const Timer& time, _In_ const BYTE c)
{
    LogInfo("KeyDown=%c", c);

    return true; 
}

bool WinInputManager::OnKeyUp(_In_ const Timer& time, _In_ const BYTE c) 
{ 
    LogInfo("KeyUp=%c", c);

    if (c == 'C')
    {
        LogInfo("Toggle Camera");
        EventPtr toggleCamera(eNEW ToggleCameraEvt(time.TotalTime()));
        g_EventMgr->Queue(toggleCamera);
    }

    return true; 
}


void WinInputManager::Update(_In_ const Timer& time)
{
}