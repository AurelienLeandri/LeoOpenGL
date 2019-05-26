#pragma once

#include <renderer/render-graph-node.hpp>

namespace leo
{
    class BlitNode : public RenderGraphNode
    {
        public:
        BlitNode(OpenGLContext &context, Framebuffer &input);
        
        public:
        virtual void render();

        private:
        Framebuffer &_input;
    };
}