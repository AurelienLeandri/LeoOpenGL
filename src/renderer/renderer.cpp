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
  this->_context.loadVAO(*v);
  this->_postProcessGeometry.addComponent(v);
}

Renderer::~Renderer()
{
  if (_mainNode != nullptr)
    delete _mainNode;
}

void Renderer::_init()
{
  this->_context.init();

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
  this->_context.setWindowContext(*window, *inputManager);
}

void Renderer::_setCamera(Camera *camera)
{
  this->_camera = camera;
}

void Renderer::render(const SceneGraph *sceneGraph)
{
  if (this->_mainNode == nullptr)
  {
    this->_mainNode = new MainNode(this->_context, *sceneGraph, this->_shader, *this->_camera);
    this->_mainNode->setOutput(&this->_main);
    this->_mainNode->_pointLights = this->_pointLights;
    this->_mainNode->_directionLights = this->_directionLights;
  }

  this->_mainNode->render();

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

  GLuint VAO = this->_context.loadCubeMap(cubeMap);
  this->_cubeMapShader.setTexture(
      "skybox", this->_context.getTextureWrapperId(*cubeMap.getTextures()[0]), 0, GL_TEXTURE_CUBE_MAP);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  glDepthFunc(GL_LESS);
}

void Renderer::_drawVolume(const Volume *volume, const Instanced *instanced)
{
  this->_context.bindVAO(*volume);
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
  this->_shader.setTexture(uniformName, this->_context.getTextureWrapperId(texture), textureSlot);
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

void Renderer::_loadInstanced(const Instanced *instanced)
{
  const std::vector<glm::mat4> &m = instanced->transformations;
  int amount = m.size();

  // vertex Buffer Object
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &m[0], GL_STATIC_DRAW);

  std::vector<const Volume *> meshes;
  this->_getChildrenMeshes(instanced->getEntity(), meshes);

  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    this->_context.loadVAOInstanced(*meshes[i]);
  }
}

void Renderer::_getChildrenMeshes(const Entity *root, std::vector<const Volume *> &meshes)
{
  const Volume *v = static_cast<const Volume *>(root->getComponent(ComponentType::VOLUME));
  if (v)
  {
    this->_context.loadVAO(*v);
    meshes.push_back(v);
  }
  for (auto p : root->getChildren())
  {
    this->_getChildrenMeshes(p.second, meshes);
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



} // namespace leo
