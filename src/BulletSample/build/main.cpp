#include <Windows.h>
#include <memory>
#include "BulletGameApp.h"

using namespace engiX;

int WINAPI wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
    std::unique_ptr<BulletGameApp> pApp(eNEW BulletGameApp);

    return BulletGameApp::Main(hPrevInstance, hPrevInstance, lpCmdLine, nCmdShow);;
}
