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

  friend class SceneGraphObserver;

  using t_id = unsigned int;

public:
  MainNode(OpenGLContext &context, const SceneGraph &sceneGraph, Shader &shader, const Camera &camera);

public:
  virtual void render();

public:
  virtual void notified(Subject *subject, Event event);

protected:
  virtual void _load();
  virtual void _unload();

private:
  void _loadShader();
  void _loadOutputFramebuffer();
  void _loadInputFramebuffers();
  void _loadTextureToShader(const char *uniformName, GLuint textureSlot, const Texture &texture);
  void _loadLightsToShader();
  void _setModelMatrix(const Transformation *transformation);
  void _setModelMatrix();
  void _setCurrentMaterial(const Material *material);
  void _renderRec(const Entity *root);
  void _drawVolume(const Volume *volume);
  void _loadLight(const DirectionLight *light);
  void _loadLight(const PointLight *light);

public: // TODO: make private
  std::map<t_id, DirectionLightUniform> _directionLights;
  std::map<t_id, PointLightUniform> _pointLights;

private:
  GLuint _lightsUBO = 0;
  GLuint _materialTextureOffset = 0;
  const SceneGraph &_sceneGraph;
  std::map<t_id, TextureWrapper> _textures;
  std::map<t_id, BufferCollection> _bufferCollections;
};

} // namespace leo