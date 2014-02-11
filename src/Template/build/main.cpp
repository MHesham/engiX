#include "stdafx.h"
#include "D3dApp.h"
#include "engiX.h"
#include "Logger.h"
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>


using namespace DirectX; 
using namespace DirectX::PackedVector;
using namespace std;
using namespace engiX;

class TemplateApp : public D3dApp
{
public:
    TemplateApp(HINSTANCE hInstance);

protected:
    void VDrawScene(); 
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
                   PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    if (!engiX::Init())
    {
        wprintf_s(L"Failed to initialize one or all of engiX subsystems");
        return 0;
    }

    LogInfo("engiX subsystems initialized successfully");

    LogInfo("Starting TemplateSample");

    TemplateApp sample(hInstance);

    if(!sample.VInit())
    {
        LogError("Failed to initialize app");
        return 0;
    }

    return sample.Run();
}

TemplateApp::TemplateApp(HINSTANCE hInstance)
    : D3dApp(hInstance)
{
    mMainWndCaption = L"Box Demo";
}

void TemplateApp::VDrawScene()
{
    md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const real*>(&Colors::LightSteelBlue));
    md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

    CHR(mSwapChain->Present(0, 0));
}
