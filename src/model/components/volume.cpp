#include "volume.hpp"

#include <tgmath.h>

namespace leo
{

namespace
{

glm::vec3 computeTangent(const glm::vec3 E1, const glm::vec3 E2, const glm::vec2 dUV1, const glm::vec2 dUV2)
{
  float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

  glm::vec3 tangent;
  tangent.x = f * (dUV2.y * E1.x - dUV1.y * E2.x);
  tangent.y = f * (dUV2.y * E1.y - dUV1.y * E2.y);
  tangent.z = f * (dUV2.y * E1.z - dUV1.y * E2.z);
  tangent = glm::normalize(tangent);

  glm::vec3 bitangent;
  bitangent.x = f * (-dUV2.x * E1.x + dUV1.x * E2.x);
  bitangent.y = f * (-dUV2.x * E1.y + dUV1.x * E2.y);
  bitangent.z = f * (-dUV2.x * E1.z + dUV1.x * E2.z);
  bitangent = glm::normalize(bitangent);

  if (tangent.x != tangent.x)
  {
    float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

    glm::vec<3, double> tangent2;
    float t1 = dUV2.y * E1.x;
    float t2 = dUV1.y * E2.x;
    float t3 = t1 - t2;
    tangent2.x = f * ((double)dUV2.y * E1.x - (double)dUV1.y * E2.x);
    tangent2.y = f * ((double)dUV2.y * E1.y - (double)dUV1.y * E2.y);
    tangent2.z = f * ((double)dUV2.y * E1.z - (double)dUV1.y * E2.z);
    tangent2 = glm::normalize(tangent2);
    return tangent2;
  }

  return tangent;
}

glm::vec3 computeBiTangent(const glm::vec3 E1, const glm::vec3 E2, const glm::vec2 dUV1, const glm::vec2 dUV2)
{
  float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

  glm::vec3 bitangent;
  bitangent.x = f * (-dUV2.x * E1.x + dUV1.x * E2.x);
  bitangent.y = f * (-dUV2.x * E1.y + dUV1.x * E2.y);
  bitangent.z = f * (-dUV2.x * E1.z + dUV1.x * E2.z);
  bitangent = glm::normalize(bitangent);

  return bitangent;
}

} // namespace

Volume::Volume(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : _vertices(vertices),
                                                                                  _indices(indices)
{
}

Volume::Volume(const Volume &other) : _indices(other._indices), _vertices(other._vertices)
{
}

Volume::Volume()
{
}

const std::string &Volume::getLabel() const
{
  return this->_label;
}

void Volume::setLabel(std::string label)
{
  this->_label = label;
}

const std::vector<Vertex> &Volume::getVertices() const
{
  return this->_vertices;
}

const std::vector<unsigned int> &Volume::getIndices() const
{
  return this->_indices;
}

Volume Volume::createCustom(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
  Volume volume(vertices, indices);
  return volume;
}

void Volume::_computeTangents()
{
  for (int i = 0; i < this->_indices.size(); i += 3)
  {
    glm::vec3 &pos1 = this->_vertices[this->_indices[i]].position;
    glm::vec3 &pos2 = this->_vertices[this->_indices[i + 1]].position;
    glm::vec3 &pos3 = this->_vertices[this->_indices[i + 2]].position;
    glm::vec2 &uv1 = this->_vertices[this->_indices[i]].texCoords;
    glm::vec2 &uv2 = this->_vertices[this->_indices[i + 1]].texCoords;
    glm::vec2 &uv3 = this->_vertices[this->_indices[i + 2]].texCoords;
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 dUV1 = uv2 - uv1;
    glm::vec2 dUV2 = uv3 - uv1;
    glm::vec3 tangent = computeTangent(edge1, edge2, dUV1, dUV2);
    glm::vec3 bitangent = computeBiTangent(edge1, edge2, dUV1, dUV2);
    if (tangent.x != tangent.x || tangent.y != tangent.y || tangent.z != tangent.z)
      tangent = glm::cross(bitangent, this->_vertices[this->_indices[i]].normal);
    if (bitangent.x != bitangent.x || bitangent.y != bitangent.y || bitangent.z != bitangent.z)
      bitangent = glm::cross(tangent, this->_vertices[this->_indices[i]].normal);
    for (int j = 0; j < 3; j++)
      this->_vertices[this->_indices[i + j]].biTangent = bitangent;
    for (int j = 0; j < 3; j++)
      this->_vertices[this->_indices[i + j]].tangent = tangent;
  }
}

Volume Volume::createCube(float side)
{
  float half = side / 2;
  std::vector<float> pos{
      -half,
      -half,
      -half,
      -half,
      half,
      -half,
      half,
      half,
      -half,
      half,
      -half,
      -half,

      half,
      half,
      half,
      -half,
      half,
      half,
      -half,
      -half,
      half,
      half,
      -half,
      half,

      -half,
      half,
      -half,
      -half,
      -half,
      -half,
      -half,
      -half,
      half,
      -half,
      half,
      half,

      half,
      half,
      half,
      half,
      -half,
      half,
      half,
      -half,
      -half,
      half,
      half,
      -half,

      half,
      -half,
      -half,
      half,
      -half,
      half,
      -half,
      -half,
      half,
      -half,
      -half,
      -half,

      -half,
      half,
      -half,
      -half,
      half,
      half,
      half,
      half,
      half,
      half,
      half,
      -half,
  };

  std::vector<float> tex{
      1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f,

      1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,

      1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f,

      1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f,

      1.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,

      0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 0.0f};

  std::vector<float> norm{
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,

      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,

      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,

      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,

      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,

      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f};
  Volume volume;
  for (int i = 0; i < 6 * 4 * 3; i += 3)
  {
    struct Vertex v;
    v.position = glm::vec3(pos[i], pos[i + 1], pos[i + 2]);
    v.normal = glm::vec3(norm[i], norm[i + 1], norm[i + 2]);
    v.texCoords = glm::vec2(tex[i], tex[i + 1]);
    volume._vertices.push_back(v);
  }

  volume._indices = std::vector<unsigned int>{
      0, 1, 3,
      1, 2, 3,
      4, 5, 7,
      5, 6, 7,
      8, 9, 11,
      9, 10, 11,
      12, 13, 15,
      13, 14, 15,
      16, 17, 19,
      17, 18, 19,
      20, 21, 23,
      21, 22, 23};

  volume._computeTangents();

  return volume;
}

Volume Volume::createSphere(float radius)
{
  unsigned int indexCount;
  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> uv;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> tangents;
  std::vector<glm::vec3> bitangents;
  std::vector<unsigned int> indices;

  const unsigned int X_SEGMENTS = 32;
  const unsigned int Y_SEGMENTS = 16;
  const double PI = 3.14159265359;
  for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
  {
    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
    {
      double xSegment = (double)x / (double)X_SEGMENTS;
      double ySegment = (double)y / (double)Y_SEGMENTS;
      double xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
      double yPos = std::cos(ySegment * PI);
      double zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
      glm::vec<3, double> p(xPos, yPos, zPos);

      positions.push_back(p);
      glm::vec<3, double> np = -glm::normalize(p);
      double u = xSegment;
      double v = ySegment;
      uv.push_back(glm::vec2(u, v));
      glm::vec<3, double> n = glm::normalize(p);
      normals.push_back(n);
    }
  }

  bool oddRow = false;
  for (int y = 0; y < Y_SEGMENTS; ++y)
  {
    if (!oddRow) // even rows: y == 0, y == 2; and so on
    {
      for (int x = 0; x <= X_SEGMENTS; ++x)
      {
        indices.push_back(y * (X_SEGMENTS + 1) + x);
        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        indices.push_back(y * (X_SEGMENTS + 1) + ((x + 1) % (X_SEGMENTS + 1)));
        indices.push_back(y * (X_SEGMENTS + 1) + ((x + 1) % (X_SEGMENTS + 1)));
        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        indices.push_back((y + 1) * (X_SEGMENTS + 1) + ((x + 1) % (X_SEGMENTS + 1)));
      }
    }
    else
    {
      for (int x = X_SEGMENTS; x >= 0; --x)
      {
        indices.push_back(y * (X_SEGMENTS + 1) + x);
        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        indices.push_back(y * (X_SEGMENTS + 1) + (x - 1 < 0 ? X_SEGMENTS : x - 1));
        indices.push_back(y * (X_SEGMENTS + 1) + (x - 1 < 0 ? X_SEGMENTS : x - 1));
        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        indices.push_back((y + 1) * (X_SEGMENTS + 1) + (x - 1 < 0 ? X_SEGMENTS : x - 1));
      }
    }
    oddRow = !oddRow;
  }
  indexCount = indices.size();

  Volume volume;

  for (int i = 0; i < positions.size(); ++i)
  {
    struct Vertex v;
    v.position = positions[i];
    if (uv.size() > 0)
    {
      v.texCoords = uv[i];
    }
    if (normals.size() > 0)
    {
      v.normal = normals[i];
    }
    volume._vertices.push_back(v);
  }

  volume._indices = indices;

  volume._computeTangents();
  return volume;
}

Volume Volume::createPlane(float width, float height)
{
  std::vector<float> pos{
      width / 2,
      0.0f,
      -height / 2,
      width / 2,
      0.0f,
      height / 2,
      -width / 2,
      0.0f,
      height / 2,
      -width / 2,
      0.0f,
      -height / 2,
  };

  std::vector<float> norm{
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
  };

  std::vector<float> texCoords{
      width,
      0.0f,
      width,
      height,
      0.0f,
      height,
      0.0f,
      0.0f,
  };

  Volume volume;
  int texIt = 0;
  for (int i = 0; i < 4 * 3; i += 3)
  {
    struct Vertex v;
    v.position = glm::vec3(pos[i], pos[i + 1], pos[i + 2]);
    v.normal = glm::vec3(norm[i], norm[i + 1], norm[i + 2]);
    v.texCoords = glm::vec2(texCoords[texIt], texCoords[texIt + 1]);
    volume._vertices.push_back(v);
    texIt += 2;
  }

  volume._indices = std::vector<unsigned int>{
      3,
      1,
      0,
      3,
      2,
      1,
  };

  volume._computeTangents();

  return volume;
}

Volume Volume::createPostProcessPlane()
{
  std::vector<float> pos{
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
  };

  std::vector<float> norm{
      0.0f,
      0.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
  };

  Volume volume;
  for (int i = 0; i < 4 * 3; i += 3)
  {
    struct Vertex v;
    v.position = glm::vec3(pos[i], pos[i + 1], pos[i + 2]);
    v.normal = glm::vec3(norm[i], norm[i + 1], norm[i + 2]);
    v.texCoords = glm::vec2(pos[i], pos[i + 1]);
    volume._vertices.push_back(v);
  }

  volume._indices = std::vector<unsigned int>{
      0,
      1,
      3,
      1,
      2,
      3,
  };

  return volume;
}

} // namespace leo
