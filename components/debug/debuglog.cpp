#include "debuglog.hpp"

#include <string_view>

namespace
{
    constexpr std::string_view levelName(Debug::Level level)
    {
        switch (level)
        {
            case Debug::Error:
                return "ERROR";
            case Debug::Warning:
                return "WARNING";
            case Debug::Info:
                return "INFO";
            case Debug::Verbose:
                return "VERBOSE";
            case Debug::Debug:
                return "DEBUG";
            case Debug::All:
                return "ALL";
        }

        return "UNKNOWN";
    }
}

Debug::Level Log::sMinDebugLevel = Debug::All;
bool Log::sWriteLevel = false;

Log::Log(Debug::Level level)
    : mShouldLog(level <= sMinDebugLevel)
{
    if (mShouldLog && sWriteLevel)
        std::cout << '[' << levelName(level) << "] ";
}

Log::~Log()
{
    if (mShouldLog)
        std::cout << std::endl;
}

Log& Log::operator<<(std::string_view rhs)
{
    if (mShouldLog)
        std::cout << rhs;

    return *this;
}

Log& Log::operator<<(const char* rhs)
{
    if (mShouldLog)
        std::cout << rhs;

    return *this;
}
