#pragma once

#include <vector>
#include <map>

namespace leo
{

class DirectionLight;
class BufferCollection;
class TextureWrapper;
class DirectionLightWrapper;
class PointLightWrapper;
class SceneGraph;
class Shader;
class OpenGLContext;

class SceneContext
{

    using t_id = unsigned int;

public:
    SceneContext(OpenGLContext &context);

public:
    void registerDirectionLight(const DirectionLight &dl, const SceneGraph &sceneGraph, Shader &shadowShader);

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