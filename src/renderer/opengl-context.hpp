#pragma once

#include <renderer/global.hpp>

namespace leo
{

typedef struct OpenGLContextOptions {

} OpenGLContextOptions;

class OpenGLContext
{
    public:
        OpenGLContext();
    
    public:
        void init();
        void init(const OpenGLContextOptions &options);

    public:
        OpenGLContext(OpenGLContext const&) = delete;
        void operator=(OpenGLContext const&) = delete;
};

} // namespace leo