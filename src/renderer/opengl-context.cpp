#include <renderer/opengl-context.hpp>

#include <renderer/debug.hpp>
#include <renderer/input-manager.hpp>
#include <renderer/framebuffer.hpp>

#include <model/components/volume.hpp>
#include <model/cube-map.hpp>

#include <utils/texture.hpp>

#include <iostream>

namespace leo
{

OpenGLContext::OpenGLContext()
{
    this->_init();
}

void OpenGLContext::_init(const OpenGLContextOptions &options)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }
    std::cerr << "GLAD initialized succesfully" << std::endl;

    // Define the viewport dimensions
    glClearColor(0.07, 0.07, 0.07, 1);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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

void OpenGLContext::setWindowContext(GLFWwindow &window, InputManager &inputManager)
{
    glfwSetWindowUserPointer(&window, &inputManager);
    glfwMakeContextCurrent(&window);
    glfwSetKeyCallback(&window, inputManager.keyCallback);
    glfwSetCursorPosCallback(&window, inputManager.mouseCallback);
    glfwSetInputMode(&window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void OpenGLContext::generateBufferCollection(BufferCollection &bc, const Volume &volume)
{
    glGenVertexArrays(1, &bc.VAO);
    glGenBuffers(1, &bc.VBO);
    glGenBuffers(1, &bc.EBO);

    const std::vector<Vertex> &vertices = volume.getVertices();
    const std::vector<GLuint> &indices = volume.getIndices();

    glBindVertexArray(bc.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, bc.VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // Vertex Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    // Tangents
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(3);
    // BiTangents
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, biTangent));
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);
}

void OpenGLContext::generateBufferCollectionInstanced(BufferCollection &bc, const Volume &volume, GLuint transformationsVBO)
{
    if (bc.VAO == 0)
        this->generateBufferCollection(bc, volume);

    unsigned int VAO = bc.VAO;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, transformationsVBO);
    // vertex Attributes
    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint OpenGLContext::generateInstancingVBO(const std::vector<glm::mat4> &transformations)
{
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, transformations.size() * sizeof(glm::mat4), &transformations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VBO;
}

void OpenGLContext::_loadBuffers(const BufferCollection &bc)
{
    glBindVertexArray(bc.VAO);
}

t_id OpenGLContext::getTextureWrapperId(const Texture &texture)
{
    auto it = this->_textures.find(texture.getId());
    if (it == this->_textures.end())
    {
        it = this->_textures.insert(std::pair<t_id, TextureWrapper>(texture.getId(), texture)).first;
    }
    return it->second.getId();
}

void OpenGLContext::loadFramebuffer(const Framebuffer *fb, GLuint bindingType)
{
    if (fb)
    {
        fb->loadFrameBuffer(bindingType);
    }
    else
    {
        glBindFramebuffer(bindingType, 0);
    }
}

GLuint OpenGLContext::loadCubeMap(const CubeMap &cubeMap)
{
    const Texture &texture = *cubeMap.getTextures()[0];
    auto it = this->_textures.find(cubeMap.getTextures()[0]->getId());
    if (it == this->_textures.end())
    {
        TextureOptions options;
        GLTextureOptions glOptions;

        if (cubeMap.getType() == CubeMapType::FACES) {
        glOptions.textureType = GL_TEXTURE_CUBE_MAP;
        glOptions.wrapping = GL_CLAMP_TO_EDGE;
        TextureWrapper &tw = this->_textures.insert(
                                                std::pair<t_id, TextureWrapper>(texture.getId(), TextureWrapper(cubeMap.getTextures(), glOptions, options)))
                                 .first->second;
        }
        else {
            glOptions.internalFormat = GL_RGB16F;
            glOptions.format = GL_RGB;
            glOptions.type = GL_FLOAT;
            glOptions.wrapping = GL_REPEAT;
            TextureWrapper &tw = this->_textures.insert(
                                                std::pair<t_id, TextureWrapper>(texture.getId(), TextureWrapper(*cubeMap.getTextures()[0], glOptions, options)))
                                 .first->second;
        }

    }

    BufferCollection &bc = this->_cubeMapBuffer;
    if (bc.VAO == 0)
    {

        glGenVertexArrays(1, &bc.VAO);
        glGenBuffers(1, &bc.VBO);

        glBindVertexArray(bc.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, bc.VBO);

        const std::vector<float> &vertices = cubeMap.getVertices();
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    }

    return bc.VAO;
}

void OpenGLContext::drawVolume(const Volume &volume, const BufferCollection &bc)
{
    this->_loadBuffers(bc);
    const std::vector<GLuint> &indices = volume.getIndices();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(),
                   GL_UNSIGNED_INT, 0);
}

void OpenGLContext::drawVolumeInstanced(const Volume &volume, const BufferCollection &bc, int amount)
{
    this->_loadBuffers(bc);
    const std::vector<GLuint> &indices = volume.getIndices();
    glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.size(),
                            GL_UNSIGNED_INT, 0, amount);
}

} // namespace leo