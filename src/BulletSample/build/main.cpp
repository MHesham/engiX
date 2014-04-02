#include "stdafx.h"
#include "engiX.h"

INT WINAPI wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
    return engiX::Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
