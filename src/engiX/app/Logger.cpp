#include "Logger.h"
#include <Windows.h>
#include "engiXDefs.h"

using namespace engiX;
using namespace std;

const wchar_t* LogTypeName[] = {
    L"E",
    L"W",
    L"I",
};

Logger* g_pLoggerInst = nullptr;

Logger* Logger::Inst()
{
    // Note that this is not thread safe
    if (g_pLoggerInst == nullptr)
        g_pLoggerInst = eNEW Logger;

    _ASSERTE(g_pLoggerInst);
    return g_pLoggerInst;
}

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

        if (m_logToFile)
        {
            _ASSERTE(!m_isLogFileInitialized);
            m_pen.open(LOG_FILENAME, ios::out);
            if (m_pen.is_open())
                m_isLogFileInitialized = true;
            _ASSERTE(m_isLogFileInitialized);
        }
        m_isInitialized = true;
    }
}
//////////////////////////////////////////////////////////////////////////
void Logger::Deinit()
{
    if (m_logToFile && m_isLogFileInitialized)
    {
        m_pen.flush();
        m_pen.close();
        m_isLogFileInitialized = false;
    }

    SAFE_DELETE(g_pLoggerInst);
}
//////////////////////////////////////////////////////////////////////////
void Logger::Log(LogType type,
    const wchar_t* pFilename,
    const wchar_t* pFuncName,
    unsigned line, 
    const wchar_t* pTxtFormat, ...)
{
    _ASSERTE(m_isInitialized);

    wchar_t buffer1[LogBufferMax];
    wchar_t buffer2[LogBufferMax];

    va_list formatArgs;
    va_start(formatArgs, pTxtFormat);
    vswprintf_s(buffer1, pTxtFormat, formatArgs);
    va_end(formatArgs);

    swprintf_s(buffer2, LogBufferMax, L"[%s] %s. %s:%d\n",
        LogTypeName[(unsigned)type],
        buffer1,
        pFuncName,
        line);

    wprintf_s(buffer2);

    if (m_logToDebugWindow && (IsDebuggerPresent() == TRUE))
        OutputDebugStringW(buffer2);

    if (m_logToFile && m_isLogFileInitialized)
        m_pen << buffer2;
}