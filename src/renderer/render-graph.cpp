#include <renderer/render-graph.hpp>

#include <iostream>

namespace leo
{

RenderGraph::~RenderGraph()
{
}

RenderGraphNode *RenderGraph::getNode(t_id id)
{
    auto it = this->_nodes.find(id);
    if (it == this->_nodes.end())
    {
        return nullptr;
    }
    return it->second.get();
}

void RenderGraph::execute()
{
    std::map<t_id, int> incompleteInputs;
    std::map<t_id, bool> hasRan;
    for (auto &pair : this->_nodes)
    {
        incompleteInputs.insert(std::pair<t_id, int>(pair.first, pair.second->getInNodes().size()));
        hasRan.insert(std::pair<t_id, int>(pair.first, false));
    }
    bool change = false;
    bool finished = false;
    while (!finished)
    {
        finished = true;
        change = false;
        for (auto &p : this->_nodes)
        {
            if (!hasRan[p.first])
            {
                if (incompleteInputs[p.first] == 0)
                {
                    p.second->render();
                    change = true;
                    hasRan[p.first] = true;
                    for (auto &output : p.second->getOutNodes())
                    {
                        incompleteInputs[output]--;
                    }
                }
                else
                {
                    finished = false;
                }
            }
        }
        if (!finished && !change)
        {
            std::cerr << "Incomplete RenderGraph!" << std::endl;
            exit(1);
        }
    }
}

} // namespace leo