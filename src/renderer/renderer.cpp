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
      glClearColor(1.07, 0.07, 0.07, 1);

      // Setup some OpenGL options
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);

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
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
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
      return this->render(root, std::vector<const Framebuffer *>());
    }

    Framebuffer &Renderer::render(model::Base *root,
        std::vector<const Framebuffer *> inputs) {
      this->_shader.use();
      this->_renderRec(root, inputs);
      glfwSwapBuffers(this->_window);
      return this->_output;
    }

    void Renderer::_renderRec(model::Base *root,
        std::vector<const Framebuffer *> inputs) {
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
          const std::vector<Vertex> &vertices = volume->getVertices();
          const std::vector<GLuint> &indices = volume->getIndices();
        }
      }
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
      UNUSED(transformation);
    }

  }  // namespace renderer
}  // namespace leo
