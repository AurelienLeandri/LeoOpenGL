#pragma once

#include <renderer/render-node.hpp>
#include <renderer/light-uniforms.hpp>

namespace leo
{

class Transformation;

class MainNode : public RenderNode
{
    using t_id = unsigned int;

  public:
    MainNode(Shader &shader, const Camera &camera);

  public:
    virtual void render();

  protected:
    virtual void _load();
    virtual void _unload();

  private:
    void _loadShader();
    void _loadOutputFramebuffer();
    void _loadInputFramebuffers();
    void _loadLightsToShader();
    void _setModelMatrix(const Transformation *transformation);
    void _setModelMatrix();

  private:
    GLuint _lightsUBO = 0;
    GLuint _materialTextureOffset = 0;
    std::map<t_id, DirectionLightUniform> _directionLights;
    std::map<t_id, PointLightUniform> _pointLights;
};

} // namespace leo