#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace leo {

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0f;


/* An camera class that processes input and calculates the corresponding Eular Angles,
 * Vectors and Matrices for use in OpenGL
 */
class Camera
{
  public:

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW,
        GLfloat pitch = PITCH);
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY,
        GLfloat upZ, GLfloat yaw, GLfloat pitch);
    ~Camera();
    Camera(const Camera &other);
    Camera& operator=(const Camera &other);

  public:
    glm::mat4 getViewMatrix() const;
    void processKeyboard(Camera_Movement direction, GLfloat deltaTime);
    void processMouse(GLfloat xoffset, GLfloat yoffset,
        GLboolean constrainPitch = true);
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();

  public:
    GLfloat getZoom() const;
    void setZoom(GLfloat zoom);
    const glm::vec3 &getPosition() const;
    void setPosition(const glm::vec3 &position);
    const glm::vec3 &getFront() const;
    void setFront(const glm::vec3 &front);
    const glm::vec3 &getUp() const;
    void setUp(const glm::vec3 &up);
    const glm::vec3 &getRight() const;
    void setRight(const glm::vec3 &right);
    const glm::vec3 &getWorldUp() const;
    void setWorldUp(const glm::vec3 &worldUp);
    GLfloat getYaw() const;
    void setYaw(GLfloat yaw);
    GLfloat getPitch() const;
    void setPitch(GLfloat pitch);
    GLfloat getMovementSpeed() const;
    void setMovementSpeed(GLfloat movementSpeed);
    GLfloat getMouseSensitivity() const;
    void setMouseSensitivity(GLfloat mouseSensitivity);

  private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    GLfloat yaw;
    GLfloat pitch;
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

};

}
