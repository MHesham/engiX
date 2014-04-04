#include "WinGameApp.h"
#include <memory>
#include "Logger.h"
#include "EventManager.h"

using namespace engiX;
using namespace std;

WinGameApp* g_pAppInst = nullptr;

WinGameApp* WinGameApp::Inst() { _ASSERTE(g_pAppInst); return g_pAppInst; }

int WinGameApp::Main(WinGameApp* pGameInst,
                     HINSTANCE hInstance,
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
    tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;		   // remember this is VERY VERY SLOW!

    //_CRTDBG_LEAK_CHECK_DF is used at program initialization to force a 
    //   leak check just before program exit. This is important because
    //   some classes may dynamically allocate memory in globally constructed
    //   objects.
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;					

    _CrtSetDbgFlag(tmpDbgFlag);

    g_Logger->Init();
    LogInfo("engiX is initializing ...");
    g_EventMgr->Init();

    DXUTSetCallbackMsgProc( WinGameApp::OnMsgProc );
    DXUTSetCallbackFrameMove( WinGameApp::OnUpdateGame );
    DXUTSetCallbackDeviceChanging( WinGameApp::ModifyDeviceSettings );
    DXUTSetCallbackD3D11DeviceAcceptable( WinGameApp::IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( WinGameApp::OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( WinGameApp::OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11SwapChainReleasing( WinGameApp::OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( WinGameApp::OnD3D11DestroyDevice );
    DXUTSetCallbackD3D11FrameRender( WinGameApp::OnD3D11FrameRender );	
    g_pApp->Init(hInstance, lpCmdLine);

    g_pApp->Run();

    LogInfo("engiX is finalizing ...");
    int exitCode = g_pApp->ExitCode();
    g_pApp->Deinit();
    g_EventMgr->Deinit();
    g_Logger->Deinit();

    _CrtDumpMemoryLeaks();

    return exitCode;
}

WinGameApp::WinGameApp()
{
    g_pAppInst = this;

    m_screenSize.cx = DEFAULT_SCREEN_WIDTH;
    m_screenSize.cy = DEFAULT_SCREEN_HEIGHT;
    m_firstUpdate = true;
}

void WinGameApp::Init(HINSTANCE hInstance, LPWSTR lpCmdLine)
{
    LogInfo("Initializing Game App");

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings(true, true);

    CHRR(DXUTInit(false, true));
    CHRR(DXUTCreateWindow(VGameAppTitle(), hInstance));
    CHRR(DXUTCreateDevice(D3D_FEATURE_LEVEL_10_1, true, m_screenSize.cx, m_screenSize.cy));
    m_pRenderer = shared_ptr<D3d11Renderer>(eNEW D3d11Renderer);

    m_gameLogic = shared_ptr<GameLogic>(VCreateLogic());
    m_gameView = shared_ptr<GameView>(VCreateStartView());
}

void WinGameApp::Deinit()
{
    LogInfo("Finalizing Game App");

    SAFE_DELETE(g_pAppInst);
}

void WinGameApp::Run()
{
    LogInfo("Starting Main Loop ...");
    // Pass control to the sample framework for handling the message pump and 
    // dispatching render calls. The sample framework will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    CHRR(DXUTMainLoop());
    DXUTShutdown();
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
LRESULT CALLBACK WinGameApp::OnMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
    LRESULT result = 0;

    if (!g_pApp)
        return result;

    switch (uMsg) 
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        {
            *pbNoFurtherProcessing = g_pApp->m_inputMgr.OnMsgProc(g_pApp->m_gameTime, uMsg, wParam, lParam);
            break;
        }
    }

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
    if (!g_pApp)
        return;

    if (g_pApp->m_firstUpdate)
        g_pApp->m_gameTime.Start();

    g_pApp->m_inputMgr.Update(g_pApp->m_gameTime);
    g_EventMgr->Update(g_pApp->m_gameTime);
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK WinGameApp::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}