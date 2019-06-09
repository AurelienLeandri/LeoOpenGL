#include <renderer/render-graph-node.hpp>

#include <map>
#include <utility>

namespace leo
{

class RenderGraph
{
public:
    RenderGraph() = default;
    RenderGraph(RenderGraph &other) = delete;
    RenderGraph(RenderGraph &&other) = delete;
    const RenderGraph &operator=(const RenderGraph &other) = delete;
    virtual ~RenderGraph();

public:
    template <typename T, typename... ARGS>
    T *createNode(ARGS &&... args)
    {
        T *node = new T(std::forward<ARGS>(args)...);
        this->_nodes.insert(std::pair<t_RGNodeId, std::unique_ptr<T>>(node->getId(), node));
        return node;
    }
    
    RenderGraphNode *getNode(t_RGNodeId id);
    void execute();

private:
    std::map<t_RGNodeId, std::unique_ptr<RenderGraphNode>> _nodes;
};

} // namespace leo