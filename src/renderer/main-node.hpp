#pragma once

#include <renderer/render-node.hpp>
#include <renderer/light-uniforms.hpp>
#include <renderer/buffer-collection.hpp>
#include <renderer/texture-wrapper.hpp>
#include <renderer/light-uniforms.hpp>

namespace leo
{

class Transformation;
class SceneGraph;
class Material;
class Entity;
class Volume;
class Texture;
class DirectionLight;
class PointLight;

class MainNode : public RenderNode
{

  using t_id = unsigned int;

public:
  MainNode(OpenGLContext &context, SceneGraph &sceneGraph, Shader &shader, const Camera &camera,
           RenderNodeOptions = {GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT});

public:
  virtual void render();
  virtual void notified(Subject *subject, Event event);
  void setLightSpaceMatrix(glm::mat4x4 lightSpaceMatrix);

protected:
  virtual void _loadShader() override;

protected:
  void _loadLightsToShader();
  void _setModelMatrix(const Transformation *transformation);
  void _setModelMatrix(const glm::mat4x4 *transformation);
  void _setModelMatrix();
  void _setCurrentMaterial(const Material *material);
  void _renderRec(const Entity *root, const Material *material, const glm::mat4x4 *matrix);
  void _loadAllLightsFromSceneGraph();
  void _loadLight(const DirectionLight *light);
  void _loadLight(const PointLight *light);

protected:
  virtual void _drawVolume(const Volume *volume);
  virtual void _loadVolume(const Volume *volume);

private: // TODO: make private
  std::map<t_id, DirectionLightUniform> _directionLights;
  std::map<t_id, PointLightUniform> _pointLights;

private:
  GLuint _lightsUBO = 0;
  const SceneGraph &_sceneGraph;
  const Camera &_camera;
  glm::mat4x4 _lightSpaceMatrix;
};

} // namespace leo