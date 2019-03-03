#include <renderer/scene-graph-observer.hpp>
#include <utils/geometry.hpp>
#include <model/components/volume.hpp>

namespace leo
{
namespace renderer
{

SceneGraphObserver::SceneGraphObserver(Renderer &renderer) : _renderer(renderer)
{
}

void SceneGraphObserver::notified(controller::Subject *c, controller::Event event)
{
    model::Volume *volume = dynamic_cast<model::Volume *>(c);
    if (volume) {
        notified(volume, event);
    }
}

void SceneGraphObserver::notified(model::Volume *volume, controller::Event event)
{
    switch (event)
    {
    case controller::Event::COMPONENT_ADDED:
        this->_loadComponent(volume);
        break;
    default:
        break;
    }
}

void SceneGraphObserver::notified(model::Entity *entity, controller::Event event)
{
}

void SceneGraphObserver::_loadComponent(model::Volume *volume)
{
    auto it = _renderer._bufferCollections.find(volume->getId());
    BufferCollection *bc;
    if (it == _renderer._bufferCollections.end())
    {
        _renderer._bufferCollections.insert(std::pair<model::t_id, BufferCollection>(volume->getId(), BufferCollection())).first;
        bc = &(*_renderer._bufferCollections.find(volume->getId())).second;

        glGenVertexArrays(1, &bc->VAO);
        glGenBuffers(1, &bc->VBO);
        glGenBuffers(1, &bc->EBO);

        const std::vector<Vertex> &vertices = volume->getVertices();
        const std::vector<GLuint> &indices = volume->getIndices();

        glBindVertexArray(bc->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, bc->VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     &vertices[0], GL_STATIC_DRAW);

        // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *)0);
        glEnableVertexAttribArray(0);
        // Vertex Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        // Vertex Texture Coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                     &indices[0], GL_STATIC_DRAW);
    }
    else
    {
        bc = &it->second;
    }
}

} // namespace renderer
} // namespace leo
