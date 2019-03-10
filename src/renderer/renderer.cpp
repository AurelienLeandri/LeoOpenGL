#include "renderer.hpp"

#include <utils/texture.hpp>

#include <renderer/input-manager.hpp>
#include <renderer/camera.hpp>
#include <renderer/debug.hpp>

#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>
#include <model/components/material.hpp>
#include <model/components/transformation.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <model/components/volume.hpp>
#include <model/components/instanced.hpp>
#include <model/type-id.hpp>
#include <model/component-manager.hpp>

#include <sstream>

namespace leo
{

Renderer::Renderer(GLFWwindow *window,
                   InputManager *inputManager,
                   Camera *camera,
                   Shader shader) : _shader(shader), _postProcessShader("resources/shaders/post-process.vertex.glsl", "resources/shaders/post-process.fragment.glsl"),
                                    _cubeMapShader("resources/shaders/cube-map.vs.glsl", "resources/shaders/cube-map.frag.glsl"),
                                    _instancingShader("resources/shaders/instancing.vs.glsl", "resources/shaders/basic.frag.glsl")
{
  this->_setWindowContext(window, inputManager);
  this->_setCamera(camera);
  this->_init();
  Volume *v = new Volume(Volume::createPlane(1.f, 1.f));
  this->_loadVAO(v);
  this->_postProcessGeometry.addComponent(v);
}

Renderer::~Renderer()
{
}

void Renderer::_init()
{
  // Initialize GLEW to setup the OpenGL Function pointers
  glewExperimental = GL_TRUE;

  // Define the viewport dimensions
  glClearColor(0.07, 0.07, 0.07, 1);

  // Setup some OpenGL options
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  if (glewInit() == GLEW_OK)
    std::cerr << "Glew initialized successfully" << std::endl;
  else
  {
    std::cerr << "Failed to initialize Glew" << std::endl;
    return;
  }

  // Set up debugging
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  }

  glGenBuffers(1, &this->_lightsUBO);

  glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
  auto size = sizeof(PointLightUniform);
  glBufferData(GL_UNIFORM_BUFFER,
               MAX_NUM_LIGHTS * (sizeof(PointLightUniform) + sizeof(DirectionLightUniform)),
               NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  this->_initFramebuffers();
}

void Renderer::_initFramebuffers()
{
  this->_main.generate();
}

void Renderer::_setWindowContext(GLFWwindow *window, InputManager *inputManager)
{
  this->_inputManager = inputManager;
  this->_window = window;
  glfwSetWindowUserPointer(this->_window, this->_inputManager);
  glfwMakeContextCurrent(this->_window);
  glfwSetKeyCallback(this->_window, this->_inputManager->keyCallback);
  glfwSetCursorPosCallback(this->_window, this->_inputManager->mouseCallback);
  glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Renderer::_setCamera(Camera *camera)
{
  this->_camera = camera;
}

void Renderer::render(const SceneGraph *sceneGraph)
{
  this->render(sceneGraph, std::vector<const Framebuffer *>(), &this->_main);
  this->_postProcess(&this->_main);
  glfwSwapBuffers(this->_window);
}

void Renderer::render(const SceneGraph *sceneGraph,
                      std::vector<const Framebuffer *> inputs, Framebuffer *output)
{
  this->_loadShader(&this->_shader, inputs, output);
  this->_renderRec(sceneGraph->getRoot(), &this->_shader, inputs, output);

  const CubeMap *cubeMap = sceneGraph->getCubeMap();
  if (cubeMap)
  {
    this->_drawCubeMap(*cubeMap, &this->_main);
  }
}

void Renderer::_loadShader(Shader *shader, std::vector<const Framebuffer *> inputs, Framebuffer *output)
{
  shader->use();
  shader->setMat4("view", this->_camera->getViewMatrix());
  shader->setMat4("projection", glm::perspective(this->_camera->getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));

  this->_loadInputFramebuffers(inputs, *shader);
  this->_loadOutputFramebuffer(output);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  this->_loadLightsToShader();

  this->_setModelMatrix(shader);
}

void Renderer::_postProcess(Framebuffer *input)
{
  if (!input)
    return;

  this->_postProcessShader.use();
  std::vector<const Framebuffer *> v;
  v.push_back(input);
  this->_loadInputFramebuffers(v, this->_postProcessShader);
  this->_loadOutputFramebuffer(nullptr);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  this->_renderRec(&this->_postProcessGeometry, &this->_shader, v, nullptr);
}

void Renderer::_renderRec(const Entity *root, Shader *shader,
                          std::vector<const Framebuffer *> inputs, Framebuffer *output, const Instanced *instanced)
{
  const IComponent *p_component;
  bool wasInstanced = instanced != nullptr;
  p_component = root->getComponent(ComponentType::INSTANCED);
  if (p_component)
  {
    instanced = static_cast<const Instanced *>(p_component);
    if (!wasInstanced)
    {
      this->_loadShader(&this->_instancingShader, inputs, output);
      shader = &this->_instancingShader;
    }
  }
  p_component = root->getComponent(ComponentType::MATERIAL);
  if (p_component)
  {
    this->_setCurrentMaterial(static_cast<const Material *>(p_component), shader);
  }
  p_component = root->getComponent(ComponentType::TRANSFORMATION);
  if (p_component)
  {
    this->_setModelMatrix(static_cast<const Transformation *>(p_component), shader);
  }
  // NOTE: Keep volume at the end as it is affected by the transform and material
  p_component = root->getComponent(ComponentType::VOLUME);
  if (p_component)
  {
    this->_drawVolume(static_cast<const Volume *>(p_component), instanced);
  }

  for (auto &child : root->getChildren())
    this->_renderRec(child.second, shader, inputs, output, instanced);
}

void Renderer::_drawCubeMap(const CubeMap &cubeMap, Framebuffer *output)
{
  this->_cubeMapShader.use();
  glm::mat4 untranslatedMatrix = glm::mat4(glm::mat3(this->_camera->getViewMatrix()));
  this->_cubeMapShader.setMat4("view", untranslatedMatrix);
  this->_cubeMapShader.setMat4("projection", glm::perspective(this->_camera->getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));
  this->_loadOutputFramebuffer(output);
  glDepthFunc(GL_LEQUAL);
  this->_loadCubeMap(cubeMap);
  glDepthFunc(GL_LESS);
}

void Renderer::_drawVolume(const Volume *volume, const Instanced *instanced)
{
  this->_bindVAO(volume);
  // Draw mesh
  if (instanced)
  {
    int amount = instanced->transformations.size();
    glDrawElementsInstanced(GL_TRIANGLES, volume->getIndices().size(), GL_UNSIGNED_INT, 0, amount);
  }
  else
  {
    const std::vector<GLuint> &indices = volume->getIndices();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(),
                   GL_UNSIGNED_INT, 0);
  }
}

void Renderer::_setCurrentMaterial(const Material *material, Shader *shader)
{
  shader->setVector3("material.diffuse_value", material->diffuse_value);
  if (material->diffuse_texture)
  {
    this->_loadTextureToShader("material.diffuse_texture", this->_materialTextureOffset + 0, *material->diffuse_texture);
  }

  shader->setVector3("material.specular_value", material->specular_value);
  shader->setInt("material.shininess", material->shininess);
  if (material->specular_texture)
  {
    this->_loadTextureToShader("material.specular_texture", this->_materialTextureOffset + 1, *material->specular_texture);
  }

  if (material->reflection_map)
  {
    this->_loadTextureToShader("material.reflection_map", this->_materialTextureOffset + 2, *material->reflection_map);
  }
}

void Renderer::_setCurrentMaterial(const Material *material)
{
  this->_setCurrentMaterial(material, &this->_shader);
}

void Renderer::_loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture)
{
  auto it = this->_textures.find(texture.getId());
  if (it == this->_textures.end())
  {
    it = this->_textures.insert(std::pair<t_id, TextureWrapper>(texture.getId(), texture)).first;
  }
  this->_shader.setTexture(uniformName, it->second.getId(), textureSlot);
}

void Renderer::_loadLightsToShader()
{
  unsigned int ubiLights = glGetUniformBlockIndex(this->_shader.getProgram(), "s1");
  if (ubiLights != GL_INVALID_INDEX)
  {
    glUniformBlockBinding(this->_shader.getProgram(), ubiLights, 1);
  }

  glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->_lightsUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, this->_lightsUBO);
  int i = 0;
  for (auto &p : this->_pointLights)
  {
    PointLightUniform &plu = p.second;
    glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLightUniform), sizeof(PointLightUniform), &plu);
    i++;
  }
  i = 0;
  unsigned int offset = MAX_NUM_LIGHTS * sizeof(PointLightUniform);
  for (auto &p : this->_directionLights)
  {
    DirectionLightUniform &dlu = p.second;
    glBufferSubData(GL_UNIFORM_BUFFER, offset + i * sizeof(DirectionLightUniform), sizeof(DirectionLightUniform), &dlu);
    i++;
  }
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::_loadOutputFramebuffer(Framebuffer *output)
{
  if (output)
  {
    output->loadFrameBuffer();
  }
  else
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

void Renderer::_loadInputFramebuffers(std::vector<const Framebuffer *> &inputs, Shader &shader)
{
  int inputNumber = 0;
  for (int j = 0; j < inputs.size(); ++j)
  {
    const std::vector<TextureWrapper> &cb = inputs[j]->getColorBuffers();
    for (GLuint i = 0; i < cb.size(); i++)
    {
      std::stringstream number;
      number << inputNumber;
      glUniform1i(glGetUniformLocation(shader.getProgram(), ("fb" + number.str()).c_str()), inputNumber);
      glActiveTexture(GL_TEXTURE0 + inputNumber);
      glBindTexture(GL_TEXTURE_2D, cb[i].getId());
      inputNumber++;
    }
  }
  this->_materialTextureOffset = inputNumber;
}

void Renderer::_setModelMatrix(const Transformation *transformation, Shader *shader)
{
  shader->setMat4("model", transformation->getTransformationMatrix());
}

void Renderer::_setModelMatrix(Shader *shader)
{
  glm::mat4 m;
  shader->setMat4("model", m);
}

/////////////////////////////////////
/*        LOADING FUNCTIONS        */
/////////////////////////////////////

void Renderer::_loadVAO(const Volume *volume)
{
  auto it = this->_bufferCollections.find(volume->getId());
  if (it == this->_bufferCollections.end())
  {
    this->_bufferCollections.insert(std::pair<t_id, BufferCollection>(volume->getId(), BufferCollection())).first;
    BufferCollection *bc = &(*this->_bufferCollections.find(volume->getId())).second;

    glGenVertexArrays(1, &bc->VAO);
    glGenBuffers(1, &bc->VBO);
    glGenBuffers(1, &bc->EBO);

    const std::vector<Vertex> &vertices = volume->getVertices();
    const std::vector<GLuint> &indices = volume->getIndices();

    glBindVertexArray(bc->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, bc->VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // Vertex Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);
  }
}

void Renderer::_loadInstanced(const Instanced *instanced)
{
  const std::vector<glm::mat4> &m = instanced->transformations;
  int amount = m.size();

  // vertex Buffer Object
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &m[0], GL_STATIC_DRAW);

  std::vector<BufferCollection *> meshes;
  this->_getChildrenMeshes(instanced->getEntity(), meshes);

  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    unsigned int VAO = meshes[i]->VAO;
    glBindVertexArray(VAO);
    // vertex Attributes
    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
  }
}

void Renderer::_getChildrenMeshes(const Entity *root, std::vector<BufferCollection *> &buffers)
{
  const Volume *v = static_cast<const Volume *>(root->getComponent(ComponentType::VOLUME));
  if (v)
  {
    auto it = this->_bufferCollections.find(v->getId());
    if (it != this->_bufferCollections.end())
    {
      buffers.push_back(&it->second);
    }
    else
    {
      this->_loadVAO(v);
      buffers.push_back(&this->_bufferCollections[v->getId()]);
    }
  }
  for (auto p : root->getChildren())
  {
    this->_getChildrenMeshes(p.second, buffers);
  }
}

void Renderer::_bindVAO(const Volume *volume)
{
  auto it = this->_bufferCollections.find(volume->getId());
  if (it == this->_bufferCollections.end())
  {
    std::cerr << "Error: buffer collection of volume ID " << volume->getId() << " not found." << std::endl;
  }
  else
  {
    glBindVertexArray(it->second.VAO);
  }
}

void Renderer::_loadLight(const DirectionLight *light)
{
  this->_directionLights.insert(std::pair<t_id, DirectionLightUniform>(light->getId(), DirectionLightUniform(*light)));
  DirectionLightUniform &dlu = this->_directionLights[light->getId()];
  const Transformation *transform = static_cast<const Transformation *>(light->getEntity()->getComponent(ComponentType::TRANSFORMATION));
  if (transform)
  {
    const glm::mat4x4 &transformation = transform->getTransformationMatrix();
    dlu.direction = transformation * light->direction;
  }
}

void Renderer::_loadLight(const PointLight *light)
{
  this->_pointLights.insert(std::pair<t_id, PointLightUniform>(light->getId(), PointLightUniform(*light)));
  PointLightUniform &plu = this->_pointLights[light->getId()];
  const Transformation *transform = static_cast<const Transformation *>(light->getEntity()->getComponent(ComponentType::TRANSFORMATION));
  if (transform)
  {
    const glm::mat4x4 &transformation = transform->getTransformationMatrix();
    plu.position = transformation * light->position;
  }
}

void Renderer::_loadCubeMap(const CubeMap &cubeMap)
{
  const Texture &texture = *cubeMap.getTextures()[0];
  auto it = this->_textures.find(cubeMap.getTextures()[0]->getId());
  if (it == this->_textures.end())
  {
    TextureWrapper &tw = this->_textures.insert(std::pair<t_id, TextureWrapper>(texture.getId(), TextureWrapper(texture, false))).first->second;
    glBindTexture(GL_TEXTURE_CUBE_MAP, tw.getId());
    for (int i = 0; i < 6; ++i)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubeMap.getTextures()[i]->data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }

  TextureWrapper &tw = this->_textures.find(texture.getId())->second;

  this->_cubeMapShader.setTexture("skybox", tw.getId(), 0, GL_TEXTURE_CUBE_MAP);

  BufferCollection &bc = this->_cubeMapBuffer;
  if (bc.VAO == 0)
  {

    glGenVertexArrays(1, &bc.VAO);
    glGenBuffers(1, &bc.VBO);

    glBindVertexArray(bc.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, bc.VBO);

    const std::vector<float> &vertices = cubeMap.getVertices();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  }

  glBindVertexArray(bc.VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

} // namespace leo
