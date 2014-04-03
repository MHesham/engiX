#include <Windows.h>
#include <memory>
#include "BulletGameApp.h"

using namespace engiX;

int WINAPI wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
    BulletGameApp* pApp = eNEW BulletGameApp;

    int ret = BulletGameApp::Main(hPrevInstance, hPrevInstance, lpCmdLine, nCmdShow);

    delete pApp;

    return ret;
}
