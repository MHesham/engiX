#include "WinGameApp.h"
#include "Logger.h"

using namespace engiX;

WinGameApp* g_pAppInst = nullptr;

WinGameApp* WinGameApp::Inst() { _ASSERTE(g_pAppInst); return g_pAppInst; }
//////////////////////////////////////////////////////////////////////////
int WinGameApp::Main(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
    // Set up checks for memory leaks.
    // Game Coding Complete reference - Chapter 21, page 834
    //
    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

    // set this flag to keep memory blocks around
    // tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;	       // this flag will cause intermittent pauses in your game and potientially cause it to run out of memory!

    // perform memory check for each alloc/dealloc
    // tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;		   // remember this is VERY VERY SLOW!

    //_CRTDBG_LEAK_CHECK_DF is used at program initialization to force a 
    //   leak check just before program exit. This is important because
    //   some classes may dynamically allocate memory in globally constructed
    //   objects.
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;					

    _CrtSetDbgFlag(tmpDbgFlag);

    g_Logger.Init();
    LogInfo("engiX is initializing ...");

    DXUTSetCallbackMsgProc( WinGameApp::MsgProc );
    DXUTSetCallbackFrameMove( WinGameApp::OnUpdateGame );
    DXUTSetCallbackDeviceChanging( WinGameApp::ModifyDeviceSettings );

    DXUTSetCallbackD3D9DeviceAcceptable( WinGameApp::IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( WinGameApp::OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( WinGameApp::OnD3D9ResetDevice );
    DXUTSetCallbackD3D9DeviceLost( WinGameApp::OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( WinGameApp::OnD3D9DestroyDevice );
    DXUTSetCallbackD3D9FrameRender( WinGameApp::OnD3D9FrameRender );

    DXUTSetCallbackD3D11DeviceAcceptable( WinGameApp::IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( WinGameApp::OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( WinGameApp::OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11SwapChainReleasing( WinGameApp::OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( WinGameApp::OnD3D11DestroyDevice );
    DXUTSetCallbackD3D11FrameRender( WinGameApp::OnD3D11FrameRender );	


    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings(true, true);

    g_pApp->Init(hInstance, lpCmdLine);

    g_pApp->Run();

    LogInfo("engiX is finalizing ...");
    g_pApp->Deinit();
    g_Logger.Deinit();

    system("pause");

    _CrtDumpMemoryLeaks();

    return g_pApp->ExitCode();
}
//////////////////////////////////////////////////////////////////////////
WinGameApp::WinGameApp()
{
    g_pAppInst = this;

    m_screenSize.cx = DEFAULT_SCREEN_WIDTH;
    m_screenSize.cy = DEFAULT_SCREEN_HEIGHT;
}
//////////////////////////////////////////////////////////////////////////
void WinGameApp::Init(HINSTANCE hInstance, LPWSTR lpCmdLine)
{
    LogInfo("Initializing Game App");

    DXUTInit(false, true); 
    DXUTCreateWindow(VGameAppTitle(), hInstance);

    DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, m_screenSize.cx, m_screenSize.cy);
    
    if (DXUTDeviceSettings().ver == DXUTDeviceVersion::DXUT_D3D9_DEVICE)
        LogInfo("engiX is running under DirectX9");
    else
        LogInfo("engiX is running under DirectX11");
}
//////////////////////////////////////////////////////////////////////////
void WinGameApp::Deinit()
{
    LogInfo("Finalizing Game App");

    DXUTShutdown();
}
//////////////////////////////////////////////////////////////////////////
void WinGameApp::Run()
{
    // Pass control to the sample framework for handling the message pump and 
    // dispatching render calls. The sample framework will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    DXUTMainLoop();
}

//----------------------------------------------------------
// Win32 Specific Message Handlers
//
// WndProc - the main message handler for the window class
//
// OnNcCreate - this is where you can set window data before it is created
// OnMove - called whenever the window moves; used to update members of g_App
// OnDeviceChange - called whenever you eject the CD-ROM.
// OnDisplayChange - called whenever the user changes the desktop settings
// OnPowerBroadcast - called whenever a power message forces a shutdown
// OnActivate - called whenever windows on the desktop change focus.
//
// Note: pUserContext added to comply with DirectX 9c - June 2005 Update
//
LRESULT CALLBACK WinGameApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
    LRESULT result = 0;

    return result;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//
// See Game Coding Complete - 4th Edition - Chapter X, page Y
//--------------------------------------------------------------------------------------
void CALLBACK WinGameApp::OnUpdateGame( double fTime, float fElapsedTime, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D11 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK WinGameApp::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}

//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, the sample framework will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//
// See Game Coding Complete - 3rd Edition - Chapter 6 - page 154
//--------------------------------------------------------------------------------------
void CALLBACK WinGameApp::OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext  )
{
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK WinGameApp::OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                                 void* pUserContext )
{
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK WinGameApp::OnD3D9DestroyDevice( void* pUserContext )
{
}

//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK WinGameApp::OnD3D9LostDevice(void* pUserContext )
{
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK WinGameApp::OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice,
                                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK WinGameApp::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                                 D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return false;
}