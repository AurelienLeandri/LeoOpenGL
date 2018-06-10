#include <scene/lights/point-light.hpp>

namespace leo {

  // TODO: refactor reduntant code

  UPointLight PointLight::createLightUniform() {
    UPointLight light;
    light.ambient = this->_ambient;
    light.constant = this->_constant;
    light.diffuse = this->_diffuse;
    light.linear = this->_linear;
    light.specular = this->_specular;
    light.quadratic = this->_quadratic;
    light.position = this->_position;
    return light;
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
