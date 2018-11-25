#pragma once

#include <model/component.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace leo {
  namespace model {

    class Transformation : public Component {
      public:
        Transformation();
        virtual ~Transformation();
        Transformation(const Transformation &other);
        const Transformation &operator=(const Transformation &other);

      public:
        const glm::vec3 &getRelativeTranslation() const;
        const glm::vec3 &getRelativeRotation() const;
        const glm::vec3 &getRelativeScaling() const;
        const glm::vec3 &getAbsoluteTranslation() const;
        const glm::vec3 &getAbsoluteRotation() const;
        const glm::vec3 &getAbsoluteScaling() const;
        const glm::mat4x4 &getTransformationMatrix() const;
        void setRelativeTranslation(glm::vec3 value);
        void setRelativeRotation(glm::vec3 value);
        void setRelativeScaling(glm::vec3 value);
        void translate(glm::vec3 value);
        void rotate(glm::vec3 value);
        void scale(glm::vec3 value);

      private:
        void _recomputeTransformationMatrix();
        std::vector<Transformation *> _getChildTransformations();

      private:
        glm::vec3 _relativeTranslation;
        glm::vec3 _relativeRotation;
        glm::vec3 _relativeScaling;
        glm::vec3 _absoluteTranslation;
        glm::vec3 _absoluteRotation;
        glm::vec3 _absoluteScaling;
        glm::mat4x4 _transformationMatrix;
    };

  }  // model
}  // leo
