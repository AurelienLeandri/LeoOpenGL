#pragma once

#include <renderer/global.hpp>

namespace leo
{

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

/* An camera class that processes input and calculates the corresponding Eular Angles,
   * Vectors and Matrices for use in OpenGL
   */
class Camera
{
public:
  Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat _yaw = YAW,
         GLfloat _pitch = PITCH);
  Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat _upX, GLfloat _upY,
         GLfloat _upZ, GLfloat _yaw, GLfloat _pitch);
  ~Camera();
  Camera(const Camera &other);
  Camera &operator=(const Camera &other);

public:
  glm::mat4 getViewMatrix() const;
  void processKeyboard(Camera_Movement direction, GLfloat deltaTime);
  void processMouse(GLfloat xoffset, GLfloat yoffset,
                    GLboolean constrainPitch = true);

private:
  // Calculates the _front vector from the Camera's (_updated) Eular Angles
  void _updateCameraVectors();

public:
  const glm::vec3 &getPosition() const { return this->_position; }
  const glm::vec3 &getFront() const { return this->_front; }
  const glm::vec3 &getUp() const { return this->_up; }
  const glm::vec3 &getRight() const { return this->_right; }
  const glm::vec3 &getWorldUp() const { return this->_worldUp; }
  const GLfloat &getYaw() const { return this->_yaw; }
  const GLfloat &getPitch() const { return this->_pitch; }
  const GLfloat &getMovementSpeed() const { return this->_movementSpeed; }
  const GLfloat &getMouseSensitivity() const { return this->_mouseSensitivity; }
  const GLfloat &getZoom() const { return this->_zoom; }
  void setZoom(const GLfloat &value) { this->_zoom = value; }
  void setPosition(const glm::vec3 &value) { this->_position = value; }
  void setFront(const glm::vec3 &value) { this->_front = value; }
  void setUp(const glm::vec3 &value) { this->_up = value; }
  void setRight(const glm::vec3 &value) { this->_right = value; }
  void setWorldUp(const glm::vec3 &value) { this->_worldUp = value; }
  void setYaw(const GLfloat &value) { this->_yaw = value; }
  void setPitch(const GLfloat &value) { this->_pitch = value; }
  void setMovementSpeed(const GLfloat &value) { this->_movementSpeed = value; }
  void setMouseSensitivity(const GLfloat &value) { this->_mouseSensitivity = value; }

private:
  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _up;
  glm::vec3 _right;
  glm::vec3 _worldUp;
  GLfloat _yaw;
  GLfloat _pitch;
  GLfloat _movementSpeed;
  GLfloat _mouseSensitivity;
  GLfloat _zoom;
};

} // namespace leo
