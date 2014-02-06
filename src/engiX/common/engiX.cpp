#include "engiX.h"
#include "Logger.h"

using namespace engiX;

bool engiX::Init()
{
    // 1. Initialize Logging Subsystem
    if (!g_Logger.Init())
    {
        wprintf_s(L"Failed to initialize logging subsystem");
        return false;
    }

    return true;
}