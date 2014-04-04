#include <Windows.h>
#include <memory>
#include "BulletGameApp.h"

using namespace engiX;

int WINAPI wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
    return BulletGameApp::Main(eNEW BulletGameApp, hInstance, hPrevInstance, lpCmdLine, nCmdShow);;
}
