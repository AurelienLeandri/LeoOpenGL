#pragma once

#include <model/components/volume.hpp>

namespace leo
{
namespace model
{
class Instanced : public Volume
{
  public:
    Instanced(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    Instanced(const Instanced &other);
    Instanced(Volume &&other);

  protected:
    Instanced();

  public:
    virtual ComponentType getTypeId() const override
    {
        return ComponentType::INSTANCED;
    }

  public:
    std::vector<glm::mat4> transformations;
};

} // namespace model
} // namespace leo