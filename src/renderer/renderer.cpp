#include "renderer.hpp"

namespace leo {
  namespace renderer {

    Renderer::Renderer() {
      this->_init();
    }

    Renderer::~Renderer() {
    }

    void Renderer::_init() {
      // Init GLFW
      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
      glfwWindowHint(GLFW_SAMPLES, 4);

      glfwSwapInterval(1);

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

    void Renderer::setWindowContext(GLFWwindow *window, InputManager *inputManager) {
      this->_inputManager = inputManager;
      this->_window = window;
      glfwSetWindowUserPointer(this->_window, this->_inputManager);
      glfwMakeContextCurrent(this->_window);
      glfwSetKeyCallback(this->_window, this->keyCallback);
      glfwSetCursorPosCallback(this->_window, this->mouseCallback);
      glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Renderer::setCamera(Camera *camera) {
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
      glfwSwapBuffers(this->_window);
      return this->_output;
    }


    // Is called whenever a key is pressed/released via GLFW
    void Renderer::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
    {
      UNUSED(scancode); UNUSED(mode);
      InputManager *im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
      if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
      if (key >= 0 && key < 1024)
      {
        if(action == GLFW_PRESS)
          im->pressKey(key);
        else if(action == GLFW_RELEASE)
          im->releaseKey(key);
      }
    }

    void Renderer::mouseCallback(GLFWwindow *window, double xpos, double ypos)
    {
      InputManager *im = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
      if(im->firstMouse) {
        im->setLastX(xpos);
        im->setLastY(ypos);
        im->setFirstMouse(false);
      }
      GLfloat xoffset = xpos - im->lastX;
      GLfloat yoffset = im->lastY - ypos;  // Reversed since y-coordinates go from bottom to left
      im->setLastX(xpos);
      im->setLastY(ypos);
      im->setXOffset(xoffset);
      im->setYOffset(yoffset);
    }

  }  // namespace renderer
}  // namespace leo
