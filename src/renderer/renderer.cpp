#include "renderer.hpp"

#include <renderer/utils/shader.hpp>
#include <model/components/material.hpp>
#include <model/scene-graph.hpp>
#include <model/cube-map.hpp>

namespace leo
{
namespace renderer
{

Renderer::Renderer(GLFWwindow *window,
                   InputManager *inputManager,
                   Camera *camera,
                   Shader shader) : _shader(shader), _postProcessShader("resources/shaders/post-process.vertex.glsl", "resources/shaders/post-process.fragment.glsl"),
                                    _cubeMapShader("resources/shaders/cube-map.vs.glsl", "resources/shaders/cube-map.frag.glsl")
{
  this->_setWindowContext(window, inputManager);
  this->_setCamera(camera);
  this->_init();
  model::Volume *v = new model::Volume(model::Volume::createPlane(1.f, 1.f));
  this->_postProcessGeometry.addComponent("Volume", v);
  model::DrawableCollection *dc = new model::DrawableCollection();
  dc->addDrawable(v);
  this->_postProcessGeometry.addComponent("DrawableCollection", dc);
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
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);

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

void Renderer::render(const model::SceneGraph *sceneGraph)
{
  this->render(sceneGraph, std::vector<const Framebuffer *>(), &this->_main);
  this->_postProcess(&this->_main);
  glfwSwapBuffers(this->_window);
}

void Renderer::render(const model::SceneGraph *sceneGraph,
                      std::vector<const Framebuffer *> inputs, Framebuffer *output)
{
  this->_shader.use();
  this->_shader.setMat4("view", this->_camera->getViewMatrix());
  this->_shader.setMat4("projection", glm::perspective(this->_camera->getZoom(), (float)1620 / (float)1080, 0.1f, 100.0f));

  this->_loadInputFramebuffers(inputs, this->_shader);
  this->_loadOutputFramebuffer(output);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  unsigned int ubiLights = glGetUniformBlockIndex(this->_shader.getProgram(), "s1");
  if (ubiLights != GL_INVALID_INDEX)
  {
    glUniformBlockBinding(this->_shader.getProgram(), ubiLights, 1);
  }
  this->_registerLightUniforms(sceneGraph->getRoot());
  glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->_lightsUBO);
  this->_loadLightsToShader();

  this->_setModelMatrix();
  this->_renderRec(sceneGraph->getRoot());

  const model::CubeMap *cubeMap = sceneGraph->getCubeMap();
  if (cubeMap)
  {
    this->_drawCubeMap(*cubeMap, &this->_main);
  }
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
  this->_renderRec(&this->_postProcessGeometry);
}

void Renderer::_renderRec(const model::Base *root)
{
  model::DrawableCollection *toDraw = nullptr;
  auto &components = root->getComponents();
  for (auto &p : components)
  {
    auto id = p.first;
    auto drawables = dynamic_cast<model::DrawableCollection *>(p.second);
    if (drawables)
    {
      toDraw = drawables;
      continue;
    }
    auto material = dynamic_cast<model::Material *>(p.second);
    if (material)
    {
      this->_setCurrentMaterial(material);
      continue;
    }
    auto transformation = dynamic_cast<model::Transformation *>(p.second);
    if (transformation)
    {
      this->_setModelMatrix(transformation);
      continue;
    }
  }
  if (toDraw)
  {
    this->_drawCollection(toDraw);
  }

  for (auto &child : root->getChildren())
    this->_renderRec(child.second);
}

void Renderer::_drawCollection(model::DrawableCollection *collection)
{
  for (auto &pair : collection->getCollection())
  {
    model::Drawable *drawable = pair.second;
    auto volume = dynamic_cast<model::Volume *>(drawable);
    if (volume)
    {
      this->_drawVolume(volume);
    }
  }
}

void Renderer::_drawCubeMap(const model::CubeMap &cubeMap, Framebuffer *output)
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

void Renderer::_loadCubeMap(const model::CubeMap &cubeMap)
{
  auto it = this->_bufferCollections.find(cubeMap.getId());
  if (it == _bufferCollections.end())
  {
    const Texture &texture = *cubeMap.getTextures()[0];
    TextureWrapper &tw = this->_textures.insert(std::pair<std::string, TextureWrapper>(texture.path, TextureWrapper(texture, false))).first->second;
    glBindTexture(GL_TEXTURE_CUBE_MAP, tw.getId());
    for (int i = 0; i < 6; ++i)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubeMap.getTextures()[i]->data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }

  TextureWrapper &tw = this->_textures.find(cubeMap.getTextures()[0]->path)->second;

  this->_cubeMapShader.setTexture("skybox", tw.getId(), 0, GL_TEXTURE_CUBE_MAP);

  auto it2 = this->_bufferCollections.find(cubeMap.getId());
  BufferCollection *bc;
  if (it2 == _bufferCollections.end()) {
    _bufferCollections.insert(std::pair<std::string, BufferCollection>(cubeMap.getId(), BufferCollection())).first;
    bc = &(*this->_bufferCollections.find(cubeMap.getId())).second;

    glGenVertexArrays(1, &bc->VAO);
    glGenBuffers(1, &bc->VBO);

    glBindVertexArray(bc->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, bc->VBO);

    const std::vector<float> &vertices = cubeMap.getVertices();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  }
  else {
    bc = &it2->second;
  }

  glBindVertexArray(bc->VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

}

void Renderer::_loadDataBuffers(const model::Volume *volume)
{
  auto it = this->_bufferCollections.find(volume->getId());
  BufferCollection *bc;
  if (it == _bufferCollections.end())
  {
    _bufferCollections.insert(std::pair<std::string, BufferCollection>(volume->getId(), BufferCollection())).first;
    bc = &(*this->_bufferCollections.find(volume->getId())).second;

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
  else
  {
    bc = &it->second;
  }
  glBindVertexArray(bc->VAO);
}

void Renderer::_drawVolume(const model::Volume *volume)
{
  this->_loadDataBuffers(volume);
  // Draw mesh
  const std::vector<GLuint> &indices = volume->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(),
                 GL_UNSIGNED_INT, 0);
}

void Renderer::_setCurrentMaterial(model::Material *material)
{
  this->_shader.setVector3("material.diffuse_value", material->diffuse_value);
  if (material->diffuse_texture)
  {
    this->_loadTextureToShader("material.diffuse_texture", this->_materialTextureOffset + 0, *material->diffuse_texture);
  }

  this->_shader.setVector3("material.specular_value", material->specular_value);
  this->_shader.setInt("material.shininess", material->shininess);
  if (material->specular_texture)
  {
    this->_loadTextureToShader("material.specular_texture", this->_materialTextureOffset + 1, *material->specular_texture);
  }

  if (material->reflection_map)
  {
    this->_loadTextureToShader("material.reflection_map", this->_materialTextureOffset + 2, *material->reflection_map);
  }
}

void Renderer::_loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture)
{
  auto it = this->_textures.find(texture.path);
  if (it == this->_textures.end())
  {
    it = this->_textures.insert(std::pair<std::string, TextureWrapper>(texture.path, texture)).first;
  }
  this->_shader.setTexture(uniformName, it->second.getId(), textureSlot);
}

void Renderer::_loadLightsToShader()
{
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

void Renderer::_registerLightUniforms(const model::Base *root)
{
  for (auto &p : root->getSceneGraph()->getDirectionLights())
  {
    this->_directionLights.insert(std::pair<std::string, DirectionLightUniform>(p.second->getId(), DirectionLightUniform(*p.second)));
    DirectionLightUniform &dlu = this->_directionLights[p.second->getId()];
    auto cTransform = p.second->getBase()->getComponents().find("Transformation");
    if (cTransform != p.second->getBase()->getComponents().end())
    {
      model::Transformation *transform = static_cast<model::Transformation *>(cTransform->second);
      const glm::mat4x4 &transformation = transform->getTransformationMatrix();
      dlu.direction = p.second->getTransformedDirection(transformation);
    }
  }
  for (auto &p : root->getSceneGraph()->getPointLights())
  {
    this->_pointLights.insert(std::pair<std::string, PointLightUniform>(p.second->getId(), PointLightUniform(*p.second)));
    PointLightUniform &plu = this->_pointLights[p.second->getId()];
    auto cTransform = p.second->getBase()->getComponents().find("Transformation");
    if (cTransform != p.second->getBase()->getComponents().end())
    {
      model::Transformation *transform = static_cast<model::Transformation *>(cTransform->second);
      const glm::mat4x4 &transformation = transform->getTransformationMatrix();
      plu.position = p.second->getTransformedPosition(transformation);
    }
  }
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

void Renderer::_setModelMatrix(model::Transformation *transformation)
{
  this->_shader.setMat4("model", transformation->getTransformationMatrix());
}

void Renderer::_setModelMatrix()
{
  glm::mat4 m;
  this->_shader.setMat4("model", m);
}

} // namespace renderer
} // namespace leo
