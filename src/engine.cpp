#include "engine.hpp"

Engine::Engine() {
  this->_init();
}

Engine::~Engine() {
  delete this->_context;
  delete this->_material;
  delete this->_camera;
}

void Engine::_init() {
  this->_context = new OpenGLContext();
  this->_material = new BasicMaterial();
  this->_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
}

void Engine::_updateCamera(Camera &camera, float deltaTime, sf::Vector2f &formerPosition) {
  sf::Vector2f newPosition(sf::Mouse::getPosition());
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    this->_camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    this->_camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    this->_camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    this->_camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);
  this->_camera.processMouse(newPosition.x - formerPosition.x,
                      formerPosition.y - newPosition.y);
  formerPosition = newPosition;
}

void Engine::gameLoop() {
  bool running = true;
  sf::Clock clock;
  sf::Vector2f mousePosition(sf::Mouse::getPosition());
  while (running) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sf::Event event;
    while (window->pollEvent(event))
      if (event.type == sf::Event::Closed)
        running = false;
    this->_updateCamera(camera, clock.getElapsedTime().asSeconds(), mousePosition);
    clock.restart();
    // Draw objects
    shader.Use();
    glm::mat4 model;
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(camera.getZoom(), (float)800/(float)600, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // Cubes
    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);  // We omit the glActiveTexture part since TEXTURE0 is already the default active texture unit. (sampler used in fragment is set to 0 as well as default)
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    model = glm::mat4();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    window->display();
  }
}
