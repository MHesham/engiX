#include "Logger.h"
#include <Windows.h>

using namespace engiX;
using namespace std;

const wchar_t* LogTypeName[] = {
    L"Warning",
    L"Error",
    L"Info",
};

void Logger::Init()
{
    _ASSERTE(!m_isInitialized);
    if (!m_isInitialized)
    {

#if defined(DEBUG) | defined(_DEBUG)
        m_logToDebugWindow = true;
#endif
        _ASSERTE(!m_isConsoleInitialized);
        m_isConsoleInitialized = m_consoleWindow.Init();
        SetConsoleTitleW(L"engiX Log");
        _ASSERTE(m_isConsoleInitialized);

        _ASSERTE(!m_isLogFileInitialized);
        m_pen.open(LOG_FILENAME, ios::out);
        if (m_pen.is_open())
            m_isLogFileInitialized = true;
        _ASSERTE(m_isLogFileInitialized);

        m_isInitialized = true;
    }
}
//////////////////////////////////////////////////////////////////////////
void Logger::Deinit()
{
    if (m_isLogFileInitialized)
    {
        m_pen.flush();
        m_pen.close();
        m_isLogFileInitialized = false;
    }
}
//////////////////////////////////////////////////////////////////////////
void Logger::Log(LogType type, const wchar_t* pFuncName, const wchar_t* pTxtFormat, ...)
{
    _ASSERTE(m_isInitialized);

    wchar_t buffer1[LogBufferMax];
    wchar_t buffer2[LogBufferMax];

    va_list formatArgs;
    va_start(formatArgs, pTxtFormat);
    vswprintf_s(buffer1, pTxtFormat, formatArgs);
    va_end(formatArgs);

    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);

    swprintf_s(buffer2, LogBufferMax, L"[%s@%02d:%02d:%02d.%03d@%s] %s\n",
        LogTypeName[(unsigned)type],
        sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds,
        pFuncName,
        buffer1);

    wprintf_s(buffer2);

    if (m_logToDebugWindow && (IsDebuggerPresent() == TRUE))
        OutputDebugStringW(buffer2);

    if (m_isLogFileInitialized)
        m_pen << buffer2;
}