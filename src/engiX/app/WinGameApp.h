#pragma once

#include "DXUT.h"
#include "engiXDefs.h"

namespace engiX
{
    class WinGameApp
    {
    public:
        static const int DEFAULT_SCREEN_WIDTH = 1024;
        static const int DEFAULT_SCREEN_HEIGHT = 768;

        static int Main(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPWSTR    lpCmdLine,
            int       nCmdShow);

        static WinGameApp* Inst();

        WinGameApp();
        void Init(HINSTANCE hInstance, LPWSTR lpCmdLine);
        void Deinit();
        void Run();
        int ExitCode() const { return DXUTGetExitCode(); }
        const SIZE& ScreenSize() const { return m_screenSize; }

    protected:
        virtual const wchar_t* VGameAppTitle() const = 0;
        HWND WindowHandle() const { return DXUTGetHWND(); }

    private:
        // DXUT General Handlers
        static LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void *pUserContext );
        static bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
        static void CALLBACK OnUpdateGame( double fTime, float fElapsedTime, void *pUserContext );

        // DXUT DirectX10 Handlers
        static bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext ) { return true; }
        static HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) { return S_OK; }
        static HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) { return S_OK; }
        static void CALLBACK OnD3D10ReleasingSwapChain( void* pUserContext ) {}
        static void CALLBACK OnD3D10DestroyDevice( void* pUserContext ) {}
        static void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext ) {}

        SIZE m_screenSize;
   };

#define g_pApp engiX::WinGameApp::Inst() 
}