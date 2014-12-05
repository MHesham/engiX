#pragma once

#include <fstream>
#include "Console.h"

namespace engiX
{

#define LOG_FILENAME L"engiXLog.txt"

    enum LogType
    {
        LOG_Error,
        LOG_Warning,
        LOG_Info,
        LOG_Verbose,
        LOG_END
    };

    class Logger
    {
    public:
        const static unsigned LogBufferMax = 1024;

        void Init();
        void Deinit();
        void Log(LogType type, const wchar_t* pFilename, const wchar_t* pFuncName, unsigned line, const wchar_t* pTxtFormat, ...);
        static Logger* Inst();
        void LogLevel(LogType level) { m_logLevel = level; }
        LogType LogLevel() const { return m_logLevel; }

    private:
        Logger() :
            m_logToDebugWindow(false),
            m_isLogFileInitialized(false),
            m_isConsoleInitialized(false),
            m_isInitialized(false),
            m_logToFile(false),
            m_logLevel(LOG_Error)
        {}

        std::wfstream m_pen;
        CConsole m_consoleWindow;
        bool m_logToDebugWindow;
        bool m_isLogFileInitialized;
        bool m_isConsoleInitialized;
        bool m_isInitialized;
        bool m_logToFile;
        LogType m_logLevel;
    };
}

#define g_Logger					engiX::Logger::Inst()

#if defined(UNICODE) | defined(_UNICODE)
#define LogMsg(Type, Format, ...)   { if ((int)engiX::Type <= g_Logger->LogLevel()) { g_Logger->Log(engiX::Type, __FILEW__, __FUNCTIONW__, __LINE__, L##Format, __VA_ARGS__); } }
#else
#define LogMsg(Type, Format, ...)   { if ((int)engiX::Type <= g_Logger->LogLevel()) { g_Logger->Log(engiX::Type, __FILE__, __FUNCTION__, __LINE__, Format, __VA_ARGS__); } }
#endif

#define LogError(Format, ...)		LogMsg(LOG_Error, Format, __VA_ARGS__)
#define LogWarning(Format, ...)		LogMsg(LOG_Warning, Format, __VA_ARGS__)
#define LogInfo(Format, ...)		LogMsg(LOG_Info, Format, __VA_ARGS__)
#define LogVerbose(Format, ...)     LogMsg(LOG_Verbose, Format, __VA_ARGS__)
