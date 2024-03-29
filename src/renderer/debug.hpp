#pragma once

#include <renderer/global.hpp>

#include <iostream>

namespace leo
{

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const void *userParam);

} // namespace leo
