#pragma once

#include <Windows.h>
#include "Timer.h"

namespace engiX
{
    class WinInputManager
    {
    public:
        WinInputManager();
        void Update(_In_ const Timer& time);
        bool OnMsgProc(_In_ const Timer& time, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

    private:
        bool OnKeyDown(_In_ const Timer& time, _In_ const BYTE c);
        bool OnKeyUp(_In_ const Timer& time, _In_ const BYTE c);
    };
}