#include "transformation.hpp"

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
          this->_relativeTranslation = value;
          this->_absoluteTranslation += value - this->_relativeTranslation;
          this->_recomputeTransformationMatrix();
        }

        void Transformation::setRelativeRotation(glm::vec3 value) {
          this->_relativeRotation = value;
          this->_absoluteRotation += value - this->_relativeRotation;
          this->_recomputeTransformationMatrix();
        }

        void Transformation::setRelativeScaling(glm::vec3 value) {
          this->_relativeScaling = value;
          this->_absoluteScaling += value - this->_relativeScaling;
          this->_recomputeTransformationMatrix();
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
