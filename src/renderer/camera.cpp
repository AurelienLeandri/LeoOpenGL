#include "camera.hpp"

namespace leo {

// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX,
    GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :
  Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch)
{}

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) :
  _front(glm::vec3(0.0f, 0.0f, -1.0f)),
  _movementSpeed(SPEED),
  _mouseSensitivity(SENSITIVTY),
  _zoom(ZOOM)
{
  this->_position = position;
  this->_worldUp = up;
  this->_yaw = yaw;
  this->_pitch = pitch;
  this->_updateCameraVectors();
}

Camera::~Camera() {
}

Camera::Camera(const Camera &other) :
  _position(other._position),
  _front(other._front),
  _up(other._up),
  _right(other._right),
  _worldUp(other._worldUp),
  _yaw(other._yaw),
  _pitch(other._pitch),
  _movementSpeed(other._movementSpeed),
  _mouseSensitivity(other._mouseSensitivity),
  _zoom(other._zoom)
{
  this->_updateCameraVectors();
}

Camera &Camera::operator=(const Camera &other) {
  this->_position = other._position;
  this->_front = other._front;
  this->_up = other._up;
  this->_right = other._right;
  this->_worldUp = other._worldUp;
  this->_yaw = other._yaw;
  this->_pitch = other._pitch;
  this->_movementSpeed = other._movementSpeed;
  this->_mouseSensitivity = other._mouseSensitivity;
  this->_zoom = other._zoom;
  this->_updateCameraVectors();
  return *this;
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(this->_position, this->_position + this->_front, this->_up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
  GLfloat velocity = this->_movementSpeed * deltaTime;
  if (direction == FORWARD)
    this->_position += this->_front * velocity;
  if (direction == BACKWARD)
    this->_position -= this->_front * velocity;
  if (direction == LEFT)
    this->_position -= this->_right * velocity;
  if (direction == RIGHT)
    this->_position += this->_right * velocity;
  if (direction == UP)
    this->_position += this->_up * velocity;
  if (direction == DOWN)
    this->_position -= this->_up * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouse(GLfloat xoffset, GLfloat yoffset,
    GLboolean constrainPitch)
{
  xoffset *= this->_mouseSensitivity;
  yoffset *= this->_mouseSensitivity;

  this->_yaw   += xoffset;
  this->_pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (this->_pitch > 89.0f)
      this->_pitch = 89.0f;
    if (this->_pitch < -89.0f)
      this->_pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Eular angles
  this->_updateCameraVectors();
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::_updateCameraVectors()
{
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
  front.y = sin(glm::radians(this->_pitch));
  front.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
  this->_front = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  this->_right = glm::normalize(glm::cross(this->_front, this->_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  this->_up    = glm::normalize(glm::cross(this->_right, this->_front));
}

}
