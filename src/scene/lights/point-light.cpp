#include <scene/lights/point-light.hpp>

namespace leo {

  // TODO: refactor reduntant code

  UPointLight::UPointLight(const PointLight *light) :
    ULight(light),
    position(light->_modelMatrix * glm::vec4(light->_position, 1.0))
  {}

  UPointLight PointLight::createLightUniform() {
    return UPointLight(this);
  }

  PointLight::PointLight(bool genMesh) : Light() {
    if (genMesh)
      this->_genMesh();
  }

  PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh) :
    Light(ambient, diffuse, specular) {
    if (genMesh)
      this->_genMesh();
    }

  PointLight::PointLight(float constant, float linear, float quadratic,
      glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool genMesh) :
    Light(constant, linear, quadratic, ambient, diffuse, specular) {
    if (genMesh)
      this->_genMesh();
    }

  void PointLight::_genMesh() {
    TransformationVisitor tVisitor;
    tVisitor.translate(glm::vec3(-0.5f, -0.5f, -0.5f));
    Mesh *mesh = new Mesh(
        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(1.0, 1.0, 1.0),
        0
        );
    this->addChild(mesh);
    tVisitor.visit(mesh);
  }

}
