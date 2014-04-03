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

    // Set Direct3D 10 callbacks
    DXUTSetCallbackD3D10DeviceAcceptable( WinGameApp::IsD3D10DeviceAcceptable );
    DXUTSetCallbackD3D10DeviceCreated( WinGameApp::OnD3D10CreateDevice );
    DXUTSetCallbackD3D10SwapChainResized( WinGameApp::OnD3D10ResizedSwapChain );
    DXUTSetCallbackD3D10FrameRender( WinGameApp::OnD3D10FrameRender );
    DXUTSetCallbackD3D10SwapChainReleasing( WinGameApp::OnD3D10ReleasingSwapChain );
    DXUTSetCallbackD3D10DeviceDestroyed( WinGameApp::OnD3D10DestroyDevice );

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

    CHRR(DXUTInit(false, true));
    CHRR(DXUTCreateWindow(VGameAppTitle(), hInstance));
    CHRR(DXUTCreateDevice(true, m_screenSize.cx, m_screenSize.cy));
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
    LogInfo("Starting Main Loop ...");
    // Pass control to the sample framework for handling the message pump and 
    // dispatching render calls. The sample framework will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    CHRR(DXUTMainLoop());
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
//--------------------------------------------------------------------------------------
void CALLBACK WinGameApp::OnUpdateGame( double fTime, float fElapsedTime, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK WinGameApp::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}