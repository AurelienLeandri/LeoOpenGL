#pragma once

#include <renderer/global.hpp>

namespace leo
{

typedef struct BufferCollection
{
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
} BufferCollection;

} // namespace leo