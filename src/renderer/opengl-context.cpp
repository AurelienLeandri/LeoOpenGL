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

void OpenGLContext::setWindowContext(GLFWwindow &window, InputManager &inputManager)
{
    glfwSetWindowUserPointer(&window, &inputManager);
    glfwMakeContextCurrent(&window);
    glfwSetKeyCallback(&window, inputManager.keyCallback);
    glfwSetCursorPosCallback(&window, inputManager.mouseCallback);
    glfwSetInputMode(&window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void OpenGLContext::loadVAO(const Volume &volume)
{
    this->_bufferCollectionsInstanced.erase(volume.getId());

    auto it = this->_bufferCollections.find(volume.getId());

    if (it == this->_bufferCollections.end())
    {
        this->_bufferCollections.insert(std::pair<t_id, BufferCollection>(volume.getId(), BufferCollection())).first;
        BufferCollection &bc = this->_bufferCollections[volume.getId()];

        this->_generateBufferCollection(bc, volume);
    }
}

void OpenGLContext::_generateBufferCollection(BufferCollection &bc, const Volume &volume)
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);
}

void OpenGLContext::loadVAOInstanced(const Volume &volume, const GLuint &transformationsVBO)
{
    auto it = this->_bufferCollectionsInstanced.find(volume.getId());

    if (it == this->_bufferCollectionsInstanced.end())
    {
        unsigned int VAO = 0;

        auto it2 = this->_bufferCollections.find(volume.getId());
        if (it2 != this->_bufferCollections.end())
        {
            this->_bufferCollectionsInstanced.insert(
                std::pair<t_id, BufferCollection>(
                    volume.getId(), it2->second));
            this->_bufferCollections.erase(volume.getId());
        }
        else
        {
            this->_bufferCollectionsInstanced.insert(
                std::pair<t_id, BufferCollection>(
                    volume.getId(), BufferCollection()));
            this->_generateBufferCollection(this->_bufferCollectionsInstanced[volume.getId()], volume);
        }
        BufferCollection &bc = this->_bufferCollectionsInstanced[volume.getId()];

        VAO = bc.VAO;

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
}

void OpenGLContext::bindVAO(const Volume &volume)
{
    auto it = this->_bufferCollections.find(volume.getId());
    if (it != this->_bufferCollections.end())
    {
        glBindVertexArray(it->second.VAO);
        return;
    }
    auto it2 = this->_bufferCollectionsInstanced.find(volume.getId());
    if (it2 != this->_bufferCollectionsInstanced.end())
    {
        glBindVertexArray(it2->second.VAO);
        return;
    }
    std::cerr << "Error: buffer collection of volume ID " << volume.getId() << " not found." << std::endl;
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
        options.textureType = GL_TEXTURE_CUBE_MAP;
        options.wrapping = GL_CLAMP_TO_EDGE;
        TextureWrapper &tw = this->_textures.insert(
                                                std::pair<t_id, TextureWrapper>(texture.getId(), TextureWrapper(cubeMap.getTextures(), options)))
                                 .first->second;
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

} // namespace leo