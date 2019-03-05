#include <model/components/instanced.hpp>
#include <utility>

namespace leo
{
namespace model
{

Instanced::Instanced(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : Volume(vertices, indices)
{
}

Instanced::Instanced(const Instanced &other) : Volume(other)
{
}

Instanced::Instanced() : Volume()
{
}

Instanced::Instanced(Volume &&other) : Volume(std::forward<Volume>(other)) {
}

} // namespace model
} // namespace leo