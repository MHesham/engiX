#include "engiX.h"
#include "Logger.h"

namespace engiX
{
    INT engiXWinMain(HINSTANCE hInstance,
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
        //tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;		   // remember this is VERY VERY SLOW!

        //_CRTDBG_LEAK_CHECK_DF is used at program initialization to force a 
        //   leak check just before program exit. This is important because
        //   some classes may dynamically allocate memory in globally constructed
        //   objects.
        tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;					

        _CrtSetDbgFlag(tmpDbgFlag);

        g_Logger.Setup();

        LogInfo("engiX is initializing start");


        LogInfo("engiX is shutting down start");

        g_Logger.Cleanup();

        system("pause");

        _CrtDumpMemoryLeaks();

        return 0;
    }
}