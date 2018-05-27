#include <global.hpp>
#include <iostream>
#include <scene/mesh.hpp>
#include <vector>

using namespace leo;

int main()
{

  // Init GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

  // Options
  glfwSwapInterval(1);


  if (glewInit() == GLEW_OK) {
    std::cerr << "Glew initialized successfully" << std::endl;
  }
  else {
    std::cerr << "Failed to initialize Glew" << std::endl;
    return 1;
  }

  // Define the viewport dimensions
  glClearColor(0.07, 0.07, 0.07, 1);
  // Initialize GLEW to setup the OpenGL Function pointers
  glewExperimental = GL_TRUE;
  // Setup some OpenGL options
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

    std::vector<leo::Vertex> vec;
    struct leo::Vertex v1;
    v1.position = glm::vec3(1.0f, 0.0f, 0.0f);
    v1.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    v1.texCoords = glm::vec2(0.0f, 1.0f);
    struct leo::Vertex v2;
    v2.position = glm::vec3(1.0f, 1.0f, 0.0f);
    v2.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    v2.texCoords = glm::vec2(0.0f, 1.0f);
    struct leo::Vertex v3;
    v3.position = glm::vec3(0.0f, 0.0f, 0.0f);
    v3.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    v3.texCoords = glm::vec2(0.0f, 1.0f);
    vec.push_back(v1);
    vec.push_back(v2);
    vec.push_back(v3);

    std::vector<GLuint> indices = std::vector<GLuint>{
      0, 1, 2
    };

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vec.size() * sizeof(leo::Vertex),
        &vec[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
        &indices[0], GL_STATIC_DRAW);
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid *) 0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid *) offsetof(Vertex, normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid *) offsetof(Vertex, texCoords));

    glBindVertexArray(0);

  Material *material = new Material("resources/shaders/vertex-basic.glsl", "resources/shaders/fragment-basic.glsl");

  material->use();

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(),
      GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  return 0;
}
