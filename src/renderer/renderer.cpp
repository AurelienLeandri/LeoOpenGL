#include "renderer.hpp"

#include <renderer/utils/shader.hpp>
#include <model/components/material.hpp>

namespace leo {
  namespace renderer {

    Renderer::Renderer(GLFWwindow *window,
        InputManager *inputManager,
        Camera *camera,
        Shader shader
        ) :
      _shader(shader)
    {
      this->_setWindowContext(window, inputManager);
      this->_setCamera(camera);
      this->_init();
    }

    Renderer::~Renderer() {
    }

    void Renderer::_init() {
      // Initialize GLEW to setup the OpenGL Function pointers
      glewExperimental = GL_TRUE;

      // Define the viewport dimensions
      glClearColor(0.07, 0.07, 0.07, 1);

      // Setup some OpenGL options
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      //glEnable(GL_CULL_FACE);
      //glCullFace(GL_BACK);

      if (glewInit() == GLEW_OK)
        std::cerr << "Glew initialized successfully" << std::endl;
      else {
        std::cerr << "Failed to initialize Glew" << std::endl;
        return;
      }

      // Set up debugging
      GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
      if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
      {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
      }

    }

    void Renderer::_setWindowContext(GLFWwindow *window, InputManager *inputManager) {
      this->_inputManager = inputManager;
      this->_window = window;
      glfwSetWindowUserPointer(this->_window, this->_inputManager);
      glfwMakeContextCurrent(this->_window);
      glfwSetKeyCallback(this->_window, this->_inputManager->keyCallback);
      glfwSetCursorPosCallback(this->_window, this->_inputManager->mouseCallback);
      glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Renderer::_setCamera(Camera *camera) {
      this->_camera = camera;
    }

    Framebuffer &Renderer::getOutput() {
      return this->_output;
    }

    Framebuffer &Renderer::render(model::Base *root) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      Framebuffer &fb = this->render(root, std::vector<const Framebuffer *>());
      glfwSwapBuffers(this->_window);
      return fb;
    }

    Framebuffer &Renderer::render(model::Base *root,
        std::vector<const Framebuffer *> inputs) {
      this->_shader.use();
      this->_shader.setMat4("view", this->_camera->getViewMatrix());
      this->_shader.setMat4("projection", glm::perspective(this->_camera->getZoom(), (float)800/(float)600, 0.1f, 100.0f));
      this->_renderRec(root, inputs);
      return this->_output;
    }

    void Renderer::_renderRec(model::Base *root,
        std::vector<const Framebuffer *> inputs) {
      this->_setModelMatrix();
      model::DrawableCollection *toDraw;
      auto &components = root->getComponents();
      for (auto &p : components) {
        auto id = p.first;
        auto drawables = dynamic_cast<model::DrawableCollection*>(p.second);
        if (drawables) {
          toDraw = drawables;
          continue;
        }
        auto material = dynamic_cast<model::Material *>(p.second);
        if (material) {
          this->_setCurrentMaterial(material);
          continue;
        }
        auto transformation = dynamic_cast<model::Transformation *>(p.second);
        if (transformation) {
          this->_setModelMatrix(transformation);
          continue;
        }
      }
      this->_drawCollection(toDraw);

      for (auto &child : root->getChildren())
        this->_renderRec(child.second, inputs);
    }

    void Renderer::_drawCollection(model::DrawableCollection *collection) {
      for (auto &pair : collection->getCollection()) {
        model::Drawable *drawable = pair.second;
        auto volume = dynamic_cast<model::Volume *>(drawable);
        if (volume) {
          this->_drawVolume(volume);
        }
      }
    }

    void Renderer::_loadDataBuffers(const model::Volume *volume) {
      auto it = this->_bufferCollections.find(volume->getId());
      BufferCollection *bc;
      if (it == _bufferCollections.end()) {
        _bufferCollections.insert(std::pair<std::string, BufferCollection>(volume->getId(), BufferCollection())).first;
        bc = &(*this->_bufferCollections.find(volume->getId())).second;

        glGenVertexArrays(1, &bc->VAO);
        glGenBuffers(1, &bc->VBO);
        glGenBuffers(1, &bc->EBO);

        const std::vector<Vertex> &vertices = volume->getVertices();
        const std::vector<GLuint> &indices = volume->getIndices();

        glBindVertexArray(bc->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, bc->VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *)0);
        glEnableVertexAttribArray(0);
        // Vertex Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        // Vertex Texture Coords
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                     &indices[0], GL_STATIC_DRAW);
      }
      else {
        bc = &it->second;
      }
      glBindVertexArray(bc->VAO);
    }

    void Renderer::_drawVolume(const model::Volume *volume) {
      this->_loadDataBuffers(volume);
      // Draw mesh
      const std::vector<GLuint> &indices = volume->getIndices();
      glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(),
                     GL_UNSIGNED_INT, 0);
    }

    void Renderer::_setCurrentMaterial(model::Material *material) {
      this->_shader.setVector3("material.diffuse_value", material->diffuse_value);
      if (material->diffuse_texture) {
        this->_shader.setTexture("material.diffuse_texture", *material->diffuse_texture, 0);
      }

      this->_shader.setVector3("material.specular_value", material->specular_value);
      this->_shader.setInt("material.shininess", material->shininess);
      if (material->specular_texture) {
        this->_shader.setTexture("material.specular_texture", *material->specular_texture, 1);
      }

      if (material->reflection_map) {
        this->_shader.setTexture("material.reflection_map", *material->reflection_map, 2);
      }
    }

    void Renderer::_setModelMatrix(model::Transformation *transformation) {
      this->_shader.setMat4("model", transformation->getTransformationMatrix());
    }

    void Renderer::_setModelMatrix() {
      glm::mat4 m;
      this->_shader.setMat4("model", m);
    }

  }  // namespace renderer
}  // namespace leo
