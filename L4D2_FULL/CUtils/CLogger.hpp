#pragma once

#include <cstdio>
#include <cstdarg>
#include <string>

enum class eLogLevel
{
    Info,
    Warning,
    Error,
    Debug,
    Dev
};

enum class eLogType
{
    ConsoleLog,
    FileLog,
    Both
};

class CLogger
{
public:    
    static void Initialize(const char* fileName = nullptr, eLogType logType = eLogType::Both) noexcept;

    static void Shutdown() noexcept;

    static void Log(eLogLevel level, const char* format, ...) noexcept;

private:
    static const char* LevelToString(eLogLevel level) noexcept;

private:
    static FILE* m_pConsoleOutBuffer;
    static FILE* m_pFileOutBuffer;

    static eLogType m_logType;
};
