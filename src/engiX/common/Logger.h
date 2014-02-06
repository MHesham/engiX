#pragma once

#include <stack>
#include "Console.h"

namespace engiX
{
    class Logger
    {
    public:
        enum LogType
        {
            LOG_Warning,
            LOG_Error,
            LOG_Info,
            LOG_PerfStart,
            LOG_PerfEnd,
            LOG_END
        };

        const static unsigned LogBufferMax  = 1024;

        bool Init();
        void Log(LogType type, const wchar_t* pFuncName, const wchar_t* pTxtFormat, ...);
        static Logger& Instance() { static Logger inst; return inst; }

    private:
        Logger() :
            m_logToConsole(false),
            m_logToDebugWindow(false),
            m_initialized(false)
        {}
        
        bool m_logToConsole;
        bool m_logToDebugWindow;
        bool m_initialized;
        CConsole m_consoleWindow;
    };
}

#define g_Logger					engiX::Logger::Instance()

#if defined(UNICODE) | defined(_UNICODE)
#define LogWarning(Format, ...)		g_Logger.Log(engiX::Logger::LOG_Warning, __FUNCTIONW__, L##Format, __VA_ARGS__)
#define LogError(Format, ...)		g_Logger.Log(engiX::Logger::LOG_Error, __FUNCTIONW__, L##Format, __VA_ARGS__)
#define LogInfo(Format, ...)		g_Logger.Log(engiX::Logger::LOG_Info, __FUNCTIONW__, L##Format, __VA_ARGS__)
#define LogStart(Format, ...)       g_Logger.Log(engiX::Logger::LOG_PerfStart, __FUNCTIONW__, L##Format, __VA_ARGS__)
#define LogEnd(Format, ...)         g_Logger.Log(engiX::Logger::LOG_PerfEnd, __FUNCTIONW__, L##Format, __VA_ARGS__)
#else
#define LogWarning(Format, ...)		g_Logger.Log(engiX::Logger::LOG_Warning, __FUNCTION__, Format, __VA_ARGS__)
#define LogError(Format, ...)		g_Logger.Log(engiX::Logger::LOG_Error, __FUNCTION__, Format, __VA_ARGS__)
#define LogInfo(Format, ...)		g_Logger.Log(engiX::Logger::LOG_Info, __FUNCTION__, Format, __VA_ARGS__)
#define LogStart(Format, ...)       g_Logger.Log(engiX::Logger::LOG_PerfStart, __FUNCTION__, Format, __VA_ARGS__)
#define LogEnd(Format, ...)         g_Logger.Log(engiX::Logger::LOG_PerfEnd, __FUNCTION__, Format, __VA_ARGS__)
#endif