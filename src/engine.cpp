#include "engine.hpp"

namespace leo {

Engine::Engine() {
  this->_init();
}

Engine::~Engine() {
  delete this->_material;
  delete this->_camera;
}

void Engine::_init() {
  this->_material = new BasicMaterial();
  this->_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  // TODO: create FolderNode
  this->_root = new CubeGeometry();
  this->_window = new sf::Window(sf::VideoMode(800, 600), "OpenGL",
      sf::Style::Default, sf::ContextSettings(32));
  this->_window->setVerticalSyncEnabled(true);
  glClearColor(0.07, 0.07, 0.07, 1);
  glEnable(GL_DEPTH_TEST);
  if (glewInit() == GLEW_OK)
    std::cout << "Glew initialized successfully" << std::endl;
  // Define the viewport dimensions
  glViewport(0, 0, 800, 600);

  // Setup some OpenGL options
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

}

void Engine::_updateCamera(float deltaTime, sf::Vector2f &formerPosition) {
  sf::Vector2f newPosition(sf::Mouse::getPosition());
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    this->_camera->processKeyboard(Camera_Movement::FORWARD, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    this->_camera->processKeyboard(Camera_Movement::BACKWARD, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    this->_camera->processKeyboard(Camera_Movement::LEFT, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    this->_camera->processKeyboard(Camera_Movement::RIGHT, deltaTime);
  this->_camera->processMouse(newPosition.x - formerPosition.x,
      formerPosition.y - newPosition.y);
  formerPosition = newPosition;
}

void Engine::gameLoop() {
  bool running = true;
  RenderVisitor render_visitor(this->_camera, this->_window);
  sf::Clock clock;
  sf::Vector2f mousePosition(sf::Mouse::getPosition());
  while (running) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sf::Event event;
    while (this->_window->pollEvent(event))
      if (event.type == sf::Event::Closed)
        running = false;
    this->_updateCamera(clock.getElapsedTime().asSeconds(), mousePosition);
    clock.restart();
    render_visitor.visit(this->_root);
  }
}

}
