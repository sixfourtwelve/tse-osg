#include "engine.hpp"

#include <exception>
#include <iostream>

int main()
{
    try
    {
        Engine engine;
        engine.go();
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
