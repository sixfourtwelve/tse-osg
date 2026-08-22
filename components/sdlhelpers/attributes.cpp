#include "attributes.hpp"
#include "error.hpp"

void setRequiredSDLAttributes()
{
    setGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    setGLAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    setGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    setGLAttribute(SDL_GL_DOUBLEBUFFER, 1);
    setGLAttribute(SDL_GL_DEPTH_SIZE, 24);
    setGLAttribute(SDL_GL_STENCIL_SIZE, 8);
}

void setGLAttribute(SDL_GLAttr attribute, int value)
{
    if (!SDL_GL_SetAttribute(attribute, value))
        throw sdlError("Failed to configure an OpenGL attribute");
}
