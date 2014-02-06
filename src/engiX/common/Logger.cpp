#ifndef LOGGER_H
#include "Logger.h"
#endif

#include <fstream>
#include <cassert>
#include <crtdefs.h>
#include <Windows.h>

using namespace engiX;

const wchar_t* LogTypeName[] = {
    L"Warning",
    L"Error",
    L"Info",
    L"Perf-Start",
    L"Perf-End"
};

bool Logger::Init()
{
    if (!m_initialized)
    {

#if defined(DEBUG) | defined(_DEBUG)
        m_logToConsole = true;
#endif

        m_logToDebugWindow = (IsDebuggerPresent() == TRUE);
        m_initialized = m_consoleWindow.Init();
    }

    return m_initialized;
}

void Logger::Log(LogType type, const wchar_t* pFuncName, const wchar_t* pTxtFormat, ...)
{
    assert(m_initialized);
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

    if (m_logToConsole)
        wprintf_s(buffer2);

    if (m_logToDebugWindow)
        OutputDebugStringW(buffer2);
}