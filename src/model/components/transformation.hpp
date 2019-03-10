#pragma once

#include <model/icomponent.hpp>

#include <renderer/global.hpp>

namespace leo
{

class Transformation : public IComponent
{
public:
  Transformation();

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

public:
  virtual ComponentType getTypeId() const override
  {
    return ComponentType::TRANSFORMATION;
  }

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

} // namespace leo
