#include "error.hpp"
#include <SDL3/SDL.h>
#include <string>

std::runtime_error sdlError(const char* operation)
{
    return std::runtime_error(std::string(operation) + ": " + SDL_GetError());
}
