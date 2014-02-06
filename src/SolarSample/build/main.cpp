#include "stdafx.h"
#include "D3dApp.h"
#include "engiX.h"
#include "Logger.h"

using namespace engiX;

class SolarSample : public D3dApp
{
public:
    SolarSample(HINSTANCE hInstance);
    ~SolarSample();

    bool VInit();
    void VOnResize();
    void VUpdateScene(const Timer& time);
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

    LogInfo("Starting SolarSample");

    SolarSample sample(hInstance);

    if(!sample.VInit())
    {
        LogError("Failed to initialize app");
        return 0;
    }

    return sample.Run();
}

SolarSample::SolarSample(HINSTANCE hInstance)
    : D3dApp(hInstance) 
{
}

SolarSample::~SolarSample()
{
}

bool SolarSample::VInit()
{
    if(!D3dApp::VInit())
        return false;

    return true;
}

void SolarSample::VOnResize()
{
    D3dApp::VOnResize();
}

void SolarSample::VUpdateScene(const Timer& time)
{
}

void SolarSample::VDrawScene()
{
    assert(md3dImmediateContext);
    assert(mSwapChain);

    md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Blue));
    md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

    HR(mSwapChain->Present(0, 0));
}
