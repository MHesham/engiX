//***************************************************************************************
// Modified version of d3dApp.h by Frank Luna (C) 2011.
//
// Simple Direct3D demo application class.  
// Make sure you link: d3d11.lib d3dx11d.lib D3DCompiler.lib D3DX11EffectsD.lib 
//                     dxerr.lib dxgi.lib dxguid.lib.
// Link d3dx11.lib and D3DX11Effects.lib for release mode builds instead
//   of d3dx11d.lib and D3DX11EffectsD.lib.
//***************************************************************************************

#include "d3dUtil.h"
#include "Timer.h"
#include <string>

namespace engiX
{
    class D3dApp
    {
    public:
        D3dApp(HINSTANCE hInstance);
        virtual ~D3dApp();
        virtual LRESULT VMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        int Run();
        virtual bool VInit();

    protected:
        HINSTANCE AppInst()const;
        HWND      MainWnd()const;
        real     AspectRatio()const;

        // Framework methods.  Derived client class overrides these methods to 
        // implement specific application requirements.

        virtual void VOnResize(); 
        virtual void VUpdateScene(const Timer& time)=0;
        virtual void VDrawScene()=0;

        // Convenience overrides for handling mouse input.
        virtual void VOnMouseDown(WPARAM btnState, int x, int y){ }
        virtual void VOnMouseUp(WPARAM btnState, int x, int y)  { }
        virtual void VOnMouseMove(WPARAM btnState, int x, int y){ }

        bool InitMainWindow();
        bool InitDirect3D();

        void CalculateFrameStats();

        HINSTANCE mhAppInst;
        HWND      mhMainWnd;
        bool      mAppPaused;
        bool      mMinimized;
        bool      mMaximized;
        bool      mResizing;
        UINT      m4xMsaaQuality;

        Timer mTimer;

        ID3D11Device* md3dDevice;
        ID3D11DeviceContext* md3dImmediateContext;
        IDXGISwapChain* mSwapChain;
        ID3D11Texture2D* mDepthStencilBuffer;
        ID3D11RenderTargetView* mRenderTargetView;
        ID3D11DepthStencilView* mDepthStencilView;
        D3D11_VIEWPORT mScreenViewport;

        // Derived class should set these in derived constructor to customize starting values.
        std::wstring mMainWndCaption;
        D3D_DRIVER_TYPE md3dDriverType;
        int mClientWidth;
        int mClientHeight;
        bool mEnable4xMsaa;
    };
}