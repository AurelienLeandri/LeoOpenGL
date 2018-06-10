#include "camera.hpp"

namespace leo {

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) :
  front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
{
  this->position = position;
  this->worldUp = up;
  this->yaw = yaw;
  this->pitch = pitch;
  this->updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX,
    GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :
  front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
  mouseSensitivity(SENSITIVTY), zoom(ZOOM)
{
  this->position = glm::vec3(posX, posY, posZ);
  this->worldUp = glm::vec3(upX, upY, upZ);
  this->yaw = yaw;
  this->pitch = pitch;
  this->updateCameraVectors();
}

Camera::~Camera() {
}

Camera::Camera(const Camera &other) :
  position(other.position),
  front(other.front),
  up(other.up),
  right(other.right),
  worldUp(other.worldUp),
  yaw(other.yaw),
  pitch(other.pitch),
  movementSpeed(other.movementSpeed),
  mouseSensitivity(other.mouseSensitivity),
  zoom(other.zoom)
{
  this->updateCameraVectors();
}

Camera &Camera::operator=(const Camera &other) {
  this->position = other.position;
  this->front = other.front;
  this->up = other.up;
  this->right = other.right;
  this->worldUp = other.worldUp;
  this->yaw = other.yaw;
  this->pitch = other.pitch;
  this->movementSpeed = other.movementSpeed;
  this->mouseSensitivity = other.mouseSensitivity;
  this->zoom = other.zoom;
  this->updateCameraVectors();
  return *this;
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(this->position, this->position + this->front, this->up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
  GLfloat velocity = this->movementSpeed * deltaTime;
  if (direction == FORWARD)
    this->position += this->front * velocity;
  if (direction == BACKWARD)
    this->position -= this->front * velocity;
  if (direction == LEFT)
    this->position -= this->right * velocity;
  if (direction == RIGHT)
    this->position += this->right * velocity;
  if (direction == UP)
    this->position += this->up * velocity;
  if (direction == DOWN)
    this->position -= this->up * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouse(GLfloat xoffset, GLfloat yoffset,
    GLboolean constrainPitch)
{
  xoffset *= this->mouseSensitivity;
  yoffset *= this->mouseSensitivity;

  this->yaw   += xoffset;
  this->pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (this->pitch > 89.0f)
      this->pitch = 89.0f;
    if (this->pitch < -89.0f)
      this->pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Eular angles
  this->updateCameraVectors();
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  front.y = sin(glm::radians(this->pitch));
  front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  this->front = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  this->up    = glm::normalize(glm::cross(this->right, this->front));
}

GLfloat Camera::getZoom() const {
  return zoom;
}

void Camera::setZoom(GLfloat zoom) {
  Camera::zoom = zoom;
}

const glm::vec3 &Camera::getPosition() const {
  return position;
}

void Camera::setPosition(const glm::vec3 &position) {
  Camera::position = position;
}

const glm::vec3 &Camera::getFront() const {
  return front;
}

void Camera::setFront(const glm::vec3 &front) {
  Camera::front = front;
}

const glm::vec3 &Camera::getUp() const {
  return up;
}

void Camera::setUp(const glm::vec3 &up) {
  Camera::up = up;
}

const glm::vec3 &Camera::getRight() const {
  return right;
}

void Camera::setRight(const glm::vec3 &right) {
  Camera::right = right;
}

const glm::vec3 &Camera::getWorldUp() const {
  return worldUp;
}

void Camera::setWorldUp(const glm::vec3 &worldUp) {
  Camera::worldUp = worldUp;
}

GLfloat Camera::getYaw() const {
  return yaw;
}

void Camera::setYaw(GLfloat yaw) {
  Camera::yaw = yaw;
}

GLfloat Camera::getPitch() const {
  return pitch;
}

void Camera::setPitch(GLfloat pitch) {
  Camera::pitch = pitch;
}

GLfloat Camera::getMovementSpeed() const {
  return movementSpeed;
}

void Camera::setMovementSpeed(GLfloat movementSpeed) {
  Camera::movementSpeed = movementSpeed;
}

GLfloat Camera::getMouseSensitivity() const {
  return mouseSensitivity;
}

void Camera::setMouseSensitivity(GLfloat mouseSensitivity) {
  Camera::mouseSensitivity = mouseSensitivity;
}

}
