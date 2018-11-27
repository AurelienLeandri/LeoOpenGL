#include "transformation.hpp"

#include <model/base.hpp>

namespace leo {
  namespace model {

    Transformation::Transformation() :
      _relativeScaling(1.0f, 1.0f, 1.0f)
    {
    }

    Transformation::~Transformation() {
    }

    Transformation::Transformation(const Transformation &other) :
      Component(other),
      _relativeTranslation(other._relativeTranslation),
      _relativeRotation(other._relativeRotation),
      _relativeScaling(other._relativeScaling),
      _absoluteTranslation(other._absoluteTranslation),
      _absoluteRotation(other._absoluteRotation),
      _absoluteScaling(other._absoluteScaling),
      _transformationMatrix(other._transformationMatrix)
    {
    }

    const Transformation &Transformation::operator=(const Transformation &other) {
      this->_relativeTranslation = other._relativeTranslation;
      this->_relativeRotation = other._relativeRotation;
      this->_relativeScaling = other._relativeScaling;
      this->_absoluteTranslation = other._absoluteTranslation;
      this->_absoluteRotation = other._absoluteRotation;
      this->_absoluteScaling = other._absoluteScaling;
      this->_transformationMatrix = other._transformationMatrix;
      this->_notify(controller::Event::COMPONENT_UPDATED);
      return *this;
    }

    const glm::vec3 &Transformation::getRelativeTranslation() const {
      return this->_relativeTranslation;
    }

    const glm::vec3 &Transformation::getRelativeRotation() const {
      return this->_relativeRotation;
    }

    const glm::vec3 &Transformation::getRelativeScaling() const {
      return this->_relativeScaling;
    }

    const glm::vec3 &Transformation::getAbsoluteTranslation() const {
      return this->_absoluteTranslation;
    }

    const glm::vec3 &Transformation::getAbsoluteRotation() const {
      return this->_absoluteRotation;
    }

    const glm::vec3 &Transformation::getAbsoluteScaling() const {
      return this->_absoluteScaling;
    }

    const glm::mat4x4 &Transformation::getTransformationMatrix() const {
      return this->_transformationMatrix;
    }

    void Transformation::setRelativeTranslation(glm::vec3 value) {
      this->_absoluteTranslation += value - this->_relativeTranslation;
      this->_relativeTranslation = value;
      this->_recomputeTransformationMatrix();
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    void Transformation::setRelativeRotation(glm::vec3 value) {
      this->_absoluteRotation += value - this->_relativeRotation;
      this->_relativeRotation = value;
      this->_recomputeTransformationMatrix();
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    void Transformation::setRelativeScaling(glm::vec3 value) {
      this->_absoluteScaling += value - this->_relativeScaling;
      this->_relativeScaling = value;
      this->_recomputeTransformationMatrix();
      this->_notify(controller::Event::COMPONENT_UPDATED);
    }

    void Transformation::translate(glm::vec3 value) {
      this->setRelativeTranslation(this->_relativeTranslation + value);
      for (auto childTransformation : this->_getChildTransformations())
        childTransformation->translate(value);
    }

    void Transformation::rotate(glm::vec3 value) {
      this->setRelativeRotation(this->_relativeRotation + value);
      for (auto childTransformation : this->_getChildTransformations())
        childTransformation->rotate(value);
    }

    void Transformation::scale(glm::vec3 value) {
      this->setRelativeScaling(this->_relativeScaling + value);
      for (auto childTransformation : this->_getChildTransformations())
        childTransformation->scale(value);
    }

    std::vector<Transformation *> Transformation::_getChildTransformations() {
      std::vector<Transformation *> childTransformations;
      for (auto &childBase : _base->getChildren()) {
        for (auto &c : childBase.second->getComponents()) {
          Transformation *childTransformation = dynamic_cast<Transformation *>(c.second.get());
          if (childTransformation)
            childTransformations.push_back(childTransformation);
        }
      }
      return childTransformations;
    }

    void Transformation::_recomputeTransformationMatrix() {
      this->_transformationMatrix = glm::mat4x4();
      glm::translate(this->_transformationMatrix, this->_absoluteTranslation);
      glm::rotate(this->_transformationMatrix, glm::degrees(this->_absoluteRotation.x),
          glm::vec3(1.0f, 0.0f, 0.0f));
      glm::rotate(this->_transformationMatrix, glm::degrees(this->_absoluteRotation.y),
          glm::vec3(0.0f, 1.0f, 0.0f));
      glm::rotate(this->_transformationMatrix, glm::degrees(this->_absoluteRotation.z),
          glm::vec3(0.0f, 0.0f, 2.0f));
      glm::scale(this->_transformationMatrix, this->_absoluteScaling);
    }

  }  // model
}  // leo
