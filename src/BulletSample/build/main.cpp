#include "stdafx.h"
#include "engiX.h"

INT WINAPI wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
    return engiX::engiXWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
