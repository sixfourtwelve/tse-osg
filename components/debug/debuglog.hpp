#pragma once

#include <iostream>

namespace Debug
{
    enum Level : unsigned
    {
        Error = 1,
        Warning = 2,
        Info = 3,
        Verbose = 4,
        Debug = 5,
        All = 6,
    };
}

class Log
{
public:
    static Debug::Level sMinDebugLevel;
    static bool sWriteLevel;

    explicit Log(Debug::Level level);
    ~Log();

    template <typename T>
    Log& operator<<(const T& rhs)
    {
        if (mShouldLog)
            std::cout << rhs;

        return *this;
    }

    Log& operator<<(std::string_view rhs);
    Log& operator<<(const char* rhs);

private:
    bool mShouldLog;
};
