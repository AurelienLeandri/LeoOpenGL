#include <SOIL.h>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"

class OpenGLContext {
  public:
    OpenGLContext();
    ~OpenGLContext();
  public:
    sf::Window *window;

};
