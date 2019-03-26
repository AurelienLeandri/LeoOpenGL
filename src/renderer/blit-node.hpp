#pragma once

#include <renderer/render-graph-node.hpp>

namespace leo
{
    class BlitNode : public RenderGraphNode
    {
        public:
        BlitNode(OpenGLContext &context);
        
        public:
        virtual void render();
    };
}