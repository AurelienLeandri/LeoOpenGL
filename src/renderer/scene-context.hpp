#pragma once

#include <vector>
#include <map>

namespace leo
{

class DirectionLight;
class PointLight;
class BufferCollection;
class TextureWrapper;
class Texture;
class TextureOptions;
class DirectionLightWrapper;
class PointLightWrapper;
class SceneGraph;
class Shader;
class OpenGLContext;
class Material;

class SceneContext
{

    using t_id = unsigned int;

public:
    SceneContext(OpenGLContext &context);

public:
    void registerDirectionLight(const DirectionLight &dl, const SceneGraph &sceneGraph, Shader &shadowShader);
    void registerPointLight(const PointLight &dl);
    void registerMaterial(const Material &m);

private:
    void registerTexture(const Texture &tex, TextureOptions textureOptions);

public:
    // SceneGraph data
    std::map<t_id, BufferCollection> bufferCollections;
    std::map<t_id, BufferCollection> bufferCollectionsInstanced;
    std::map<t_id, TextureWrapper> textures;
    std::map<t_id, DirectionLightWrapper> dLights;
    std::map<t_id, PointLightWrapper> pLights;

    OpenGLContext &_context;
};

} // namespace leo