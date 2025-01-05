#include <Windows.h>
#include <iostream>

#include "CLogger.hpp"

FILE* CLogger::m_pConsoleOutBuffer  = nullptr;
FILE* CLogger::m_pFileOutBuffer     = nullptr;

eLogType CLogger::m_logType         = eLogType::ConsoleLog;

void CLogger::Initialize(const char* fileName, eLogType logType) noexcept
{
    if (!AllocConsole())
    {
        MessageBoxA(nullptr, "Failed to AllocConsole. Process will be closed after this message.", "Logger", MB_OK);
        ExitProcess(0x0); //Todo:
        return;
    }

    m_logType = logType;

    switch (m_logType)
    {
        case eLogType::ConsoleLog:
            if (!freopen_s(&m_pConsoleOutBuffer, "CONOUT$", "w", stdout))
            {
                CLogger::Log(eLogLevel::Info, "Console logger started.");
            }
            break;

        case eLogType::FileLog:
            if (fileName)
            {
                if (fopen_s(&m_pFileOutBuffer, fileName, "a") != 0)
                {
                    CLogger::Log(eLogLevel::Info, "File logger started.");
                }
            }
            break;
    }
}

void CLogger::Shutdown() noexcept
{
    if (m_pFileOutBuffer)
    {
        CLogger::Log(eLogLevel::Info, "File logger finished.");
        fclose(m_pFileOutBuffer);
        m_pFileOutBuffer = nullptr;
    }

    if (m_pConsoleOutBuffer)
    {
        CLogger::Log(eLogLevel::Info, "Console logger finished.");
        fclose(m_pConsoleOutBuffer);
        m_pConsoleOutBuffer = nullptr;
    }

    FreeConsole();
}

const char* CLogger::LevelToString(eLogLevel level) noexcept
{
    switch (level)
    {
        case eLogLevel::Info:
            return "[ INFO ]    ";
        case eLogLevel::Warning:
            return "[ WARNING ] ";
        case eLogLevel::Error:
            return "[ ERROR ]   ";
        case eLogLevel::Debug:
            return "[ DEBUG ]   ";
        case eLogLevel::Dev:
            return "[ DEV ]     ";
        default:
            return "[ UNKNOWN ] ";
    }
}

void CLogger::Log(eLogLevel level, const char* format, ...) noexcept
{
    switch (m_logType)
    {
        case eLogType::ConsoleLog:
        {
            va_list args;
            va_start(args, format);

            if (m_pConsoleOutBuffer)
            {
                fprintf(stdout, "%s", LevelToString(level));
                vfprintf(stdout, format, args);
                fprintf(stdout, "\n");
                fflush(stdout);
            }

            va_end(args);

            break;
        }
        
        case eLogType::FileLog:
        {
            va_list argsFile;
            va_start(argsFile, format);

            if (m_pFileOutBuffer)
            {
                fprintf(m_pFileOutBuffer, "%s", LevelToString(level));
                vfprintf(m_pFileOutBuffer, format, argsFile);
                fprintf(m_pFileOutBuffer, "\n");
                fflush(m_pFileOutBuffer);
            }

            va_end(argsFile);
            break;
        }

        case eLogType::Both:
        {
            va_list args;
            va_start(args, format);

            if (m_pConsoleOutBuffer)
            {
                fprintf(stdout, "%s", LevelToString(level));
                vfprintf(stdout, format, args);
                fprintf(stdout, "\n");
                fflush(stdout);
            }

            va_end(args);

            va_list argsFile;
            va_start(argsFile, format);

            if (m_pFileOutBuffer)
            {
                fprintf(m_pFileOutBuffer, "%s", LevelToString(level));
                vfprintf(m_pFileOutBuffer, format, argsFile);
                fprintf(m_pFileOutBuffer, "\n");
                fflush(m_pFileOutBuffer);
            }

            va_end(argsFile);

            break;
        }

        default:
            //Todo:
            MessageBoxA(nullptr, "CLogger::Log -> default.", "CLogger error", MB_OK);
        break;
    }
}
