#include "WinGameApp.h"
#include <memory>
#include <sstream>
#include "Logger.h"
#include "EventManager.h"
#
using namespace engiX;
using namespace std;

WinGameApp* g_pAppInst = nullptr;

WinGameApp* WinGameApp::Inst() { _ASSERTE(g_pAppInst); return g_pAppInst; }

int WinGameApp::Main (WinGameApp* pGameInst,
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
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

    g_Logger->Init();
    LogInfo("engiX is initializing ...");
    g_EventMgr->Init();

    DXUTSetCallbackMsgProc( WinGameApp::OnMsgProc, (void*)g_pApp);
    DXUTSetCallbackFrameMove( WinGameApp::OnUpdateGame, (void*)g_pApp);
    DXUTSetCallbackDeviceChanging( WinGameApp::ModifyDeviceSettings, (void*)g_pApp);
    DXUTSetCallbackD3D11DeviceAcceptable( WinGameApp::IsD3D11DeviceAcceptable, (void*)g_pApp);
    DXUTSetCallbackD3D11DeviceCreated( WinGameApp::OnD3D11CreateDevice, (void*)g_pApp);
    DXUTSetCallbackD3D11SwapChainResized( WinGameApp::OnD3D11ResizedSwapChain, (void*)g_pApp);
    DXUTSetCallbackD3D11SwapChainReleasing( WinGameApp::OnD3D11ReleasingSwapChain, (void*)g_pApp);
    DXUTSetCallbackD3D11DeviceDestroyed( WinGameApp::OnD3D11DestroyDevice, (void*)g_pApp);
    DXUTSetCallbackD3D11FrameRender( WinGameApp::OnD3D11FrameRender, (void*)g_pApp);	

    if (g_pApp->Init(hInstance, lpCmdLine))
        g_pApp->Run();

    LogInfo("engiX is finalizing ...");
    int exitCode = g_pApp->ExitCode();
    g_pApp->Deinit();
    g_EventMgr->Deinit();
    g_Logger->Deinit();

    // Dump DirectX object life states to catch leaking ones
    if (DXUTGetD3D11Device())
    {
        ID3D11Debug* m_d3dDebug = nullptr;
        DXUTGetD3D11Device()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_d3dDebug));

		if (m_d3dDebug)
		{
			m_d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			SAFE_RELEASE(m_d3dDebug);
		}
    }

    // Don't call any DXUT* method after the sthudown.
    // After shutting down DXUT, any call to DXUT* methods may yield an unexpected behaivor
    // e.g DXUTGetD3D11Device() will return null
    DXUTShutdown();

    _CrtDumpMemoryLeaks();

    return exitCode;
}

WinGameApp::WinGameApp()
{
    g_pAppInst = this;

    m_pGameLogic = nullptr;
    m_screenSize.cx = DEFAULT_SCREEN_WIDTH;
    m_screenSize.cy = DEFAULT_SCREEN_HEIGHT;
    m_firstUpdate = true;
}

bool WinGameApp::Init(HINSTANCE hInstance, LPWSTR lpCmdLine)
{
    LogInfo("Initializing Game App");

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings(true, true);

    m_pGameLogic = CreateLogicAndStartView();
    CBRB(m_pGameLogic->Init());

    CHRRB(DXUTInit(false, true));
    CHRRB(DXUTCreateWindow(GameAppTitle(), hInstance));
	CHRRB(DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, m_screenSize.cx, m_screenSize.cy));

    return true;
}

void WinGameApp::Deinit()
{
    LogInfo("Finalizing Game App");

    SAFE_DELETE(m_pGameLogic);
    SAFE_DELETE(g_pAppInst);
}

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
LRESULT CALLBACK WinGameApp::OnMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
    WinGameApp* pApp = (WinGameApp*)pUserContext;
    _ASSERTE(pApp);

    LRESULT result = 0;

    if (!pApp)
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
            _ASSERTE(pApp->m_pGameLogic->View());
            *pbNoFurtherProcessing = pApp->m_pGameLogic->View()->OnMsgProc(pApp->m_gameTime, uMsg, wParam, lParam);
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
    WinGameApp* pApp = (WinGameApp*)pUserContext;
    _ASSERTE(pApp);

    if (!pApp)
        return;

    if (pApp->m_firstUpdate)
    {
        pApp->m_gameTime.Reset();
        pApp->m_firstUpdate = false;
    }

    pApp->m_gameTime.Tick();

    // 1. Dispatch engine events
    g_EventMgr->OnUpdate(pApp->m_gameTime);

    // 2. Update game logic
    _ASSERTE(pApp->m_pGameLogic);
    pApp->m_pGameLogic->OnUpdate(pApp->m_gameTime);
}

//--------------------------------------------------------------------------------------
// Called right before creating a D3D device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK WinGameApp::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}

HRESULT CALLBACK WinGameApp::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    WinGameApp* pApp = (WinGameApp*)pUserContext;
    _ASSERTE(pApp);

    pApp->m_screenSize.cx = pBackBufferSurfaceDesc->Width;
    pApp->m_screenSize.cy = pBackBufferSurfaceDesc->Height;

    if (pApp->m_pGameLogic)
    {
        _ASSERTE(pApp->m_pGameLogic->View());
        pApp->m_pGameLogic->View()->OnConstruct();
    }

    g_EventMgr->Queue(EventPtr(eNEW DisplaySettingsChangedEvt(pApp->m_gameTime.TotalTime())));

    return S_OK;
}

void CALLBACK WinGameApp::OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext )
{
    WinGameApp* pApp = (WinGameApp*)pUserContext;
    _ASSERTE(pApp);

    pApp->CalcAndDisplayFrameStatistics();

    _ASSERTE(pApp->m_pGameLogic->View());
    pApp->m_pGameLogic->View()->OnRender();
}

void WinGameApp::CalcAndDisplayFrameStatistics()
{
    std::wostringstream outs;   
    outs.precision(6);
    outs << GameAppTitle() << L"    "
        << DXUTGetFrameStats(true);
    SetWindowText(DXUTGetHWND(), outs.str().c_str());
}