#include <renderer/opengl-context.hpp>

#include<renderer/debug.hpp>

#include <iostream>

namespace leo
{

OpenGLContext::OpenGLContext()
{
}

void OpenGLContext::init()
{
    this->init({});
}

void OpenGLContext::init(const OpenGLContextOptions &options)
{
    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;

    // Define the viewport dimensions
    glClearColor(0.07, 0.07, 0.07, 1);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (glewInit() == GLEW_OK)
        std::cerr << "Glew initialized successfully" << std::endl;
    else
    {
        std::cerr << "Failed to initialize Glew" << std::endl;
        return;
    }

    // Set up debugging
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

} // namespace leo