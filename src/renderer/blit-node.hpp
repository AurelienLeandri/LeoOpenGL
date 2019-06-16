#pragma once

#include <renderer/render-graph-node.hpp>

namespace leo
{
    class BlitNode : public RenderGraphNode
    {
        public:
        BlitNode(OpenGLContext &context, Framebuffer &input, GLuint blitMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        public:
        virtual void render();

        private:
        GLuint _blitMask;
        Framebuffer &_input;
    };
}