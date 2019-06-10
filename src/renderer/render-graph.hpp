#pragma once

#include <renderer/render-graph-node.hpp>
#include <renderer/shader.hpp>
#include <renderer/framebuffer.hpp>

#include <map>
#include <utility>

namespace leo
{

class RenderGraph
{
public:
    RenderGraph();
    RenderGraph(RenderGraph &other) = delete;
    RenderGraph(RenderGraph &&other) = delete;
    const RenderGraph &operator=(const RenderGraph &other) = delete;
    virtual ~RenderGraph();

public:
    template <typename... ARGS>
    Framebuffer *createFramebuffer(ARGS &&... args)
    {
        Framebuffer *fb = new Framebuffer(std::forward<ARGS>(args)...);
        this->_framebuffers.insert(std::pair<t_id, std::unique_ptr<Framebuffer>>(fb->getId(), fb));
        return fb;
    }

    template <typename... ARGS>
    Shader *createShader(ARGS &&... args)
    {
        Shader *shader = new Shader(std::forward<ARGS>(args)...);
        this->_shaders.insert(std::pair<t_id, std::unique_ptr<Shader>>(shader->getId(), shader));
        return shader;
    }

    template <typename T, typename... ARGS>
    T *createNode(ARGS &&... args)
    {
        T *node = new T(std::forward<ARGS>(args)...);
        this->_nodes.insert(std::pair<t_id, std::unique_ptr<T>>(node->getId(), node));
        return node;
    }

    RenderGraphNode *getNode(t_id id);
    Shader *getShader(t_id id);
    Framebuffer *getFramebuffer(t_id id);
    OpenGLContext &getOpenGLContext();
    SceneContext &getSceneContext();
    void execute();

private:
    std::map<t_id, std::unique_ptr<RenderGraphNode>> _nodes;
    std::map<t_id, std::unique_ptr<Framebuffer>> _framebuffers;
    std::map<t_id, std::unique_ptr<Shader>> _shaders;
    OpenGLContext _context;
    SceneContext _sceneContext;
};

} // namespace leo