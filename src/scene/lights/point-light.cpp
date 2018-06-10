#include <scene/lights/point-light.hpp>

namespace leo {

  PointLightUniform::PointLightUniform() {}

  PointLightUniform::PointLightUniform(const PointLight &light) :
    LightUniform(light)
  {
    this->position = light._position;
  }

  PointLightUniform::~PointLightUniform() {}

  PointLightUniform::PointLightUniform(const PointLightUniform &other) :
    LightUniform::LightUniform(other)
  {
    this->position = other.position;
  }

  PointLightUniform &PointLightUniform::operator=(const PointLightUniform &other) {
    LightUniform::operator=(other);
    this->position = other.position;
    return *this;
  }

  PointLight::PointLight(bool genMesh) :
    Light(),
    _genMesh(genMesh)
  {
    this->_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (this->_genMesh)
      this->_generateMesh();
  }

  PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh) :
    Light(ambient, diffuse, specular),
    _genMesh(genMesh)
  {
    this->_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (this->_genMesh)
      this->_generateMesh();
  }

  PointLight::PointLight(float constant, float linear, float quadratic,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh) :
    Light(constant, linear, quadratic, ambient, diffuse, specular),
    _genMesh(genMesh)
  {
    this->_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (this->_genMesh)
      this->_generateMesh();
  }

  PointLight::PointLight(const PointLight &other) :
    Light(other),
    _position(other._position),
    _genMesh(other._genMesh)
  {
    if (this->_genMesh)
      this->_generateMesh();
  }

  PointLight &PointLight::operator=(const PointLight &other) {
    Light::operator=(other);
    this->_position = other._position;
    this->_genMesh = other._genMesh;
    if (this->_genMesh)
      this->_generateMesh();
    return *this;
  }

  void PointLight::_generateMesh() {
    TransformationVisitor tVisitor;
    tVisitor.translate(glm::vec3(-0.5f, -0.5f, -0.5f));
    Mesh *mesh = new Mesh(
        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 0.0, 0.0),
        30
        );
    this->addChild(mesh);
    tVisitor.visit(mesh);
  }

  void PointLight::transform(const glm::mat4x4 &transformation) {
    GeometryNode::transform(transformation);
    this->_position = transformation * this->_position;
  }

}
