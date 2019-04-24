#include "transformation.hpp"

#include <model/entity.hpp>

namespace leo
{

Transformation::Transformation() : _relativeScaling(1.0f, 1.0f, 1.0f),
                                   _absoluteScaling(1.0f, 1.0f, 1.0f)
{
}

const glm::vec3 &Transformation::getRelativeTranslation() const
{
  return this->_relativeTranslation;
}

const glm::vec3 &Transformation::getRelativeRotation() const
{
  return this->_relativeRotation;
}

const glm::vec3 &Transformation::getRelativeScaling() const
{
  return this->_relativeScaling;
}

const glm::vec3 &Transformation::getAbsoluteTranslation() const
{
  return this->_absoluteTranslation;
}

const glm::vec3 &Transformation::getAbsoluteRotation() const
{
  return this->_absoluteRotation;
}

const glm::vec3 &Transformation::getAbsoluteScaling() const
{
  return this->_absoluteScaling;
}

const glm::mat4x4 &Transformation::getTransformationMatrix() const
{
  return this->_transformationMatrix;
}

void Transformation::setRelativeTranslation(glm::vec3 value)
{
  this->_absoluteTranslation += value - this->_relativeTranslation;
  this->_relativeTranslation = value;
  this->_recomputeTransformationMatrix();
  this->_notify(Event::COMPONENT_UPDATED);
}

void Transformation::setRelativeRotation(glm::vec3 value)
{
  value = glm::radians(value);
  this->_absoluteRotation += value - this->_relativeRotation;
  this->_relativeRotation = value;
  this->_recomputeTransformationMatrix();
  this->_notify(Event::COMPONENT_UPDATED);
}

void Transformation::setRelativeScaling(glm::vec3 value)
{
  this->_absoluteScaling /= this->_relativeScaling;
  this->_relativeScaling = value;
  this->_absoluteScaling *= this->_relativeScaling;
  this->_recomputeTransformationMatrix();
  this->_notify(Event::COMPONENT_UPDATED);
}

void Transformation::translate(glm::vec3 value)
{
  this->setRelativeTranslation(this->_relativeTranslation + value);
  for (auto childTransformation : this->_getChildTransformations())
    childTransformation->translate(value);
}

void Transformation::rotate(glm::vec3 value)
{
  this->setRelativeRotation(this->_relativeRotation + value);
  for (auto childTransformation : this->_getChildTransformations())
    childTransformation->rotate(value);
}

void Transformation::scale(glm::vec3 value)
{
  this->setRelativeScaling(value);
  for (auto childTransformation : this->_getChildTransformations())
    childTransformation->scale(value);
}

std::vector<Transformation *> Transformation::_getChildTransformations()
{
  std::vector<Transformation *> childTransformations;
  for (auto &childEntity : _entity->getChildren())
  {
    for (auto &c : childEntity.second->getComponents())
    {
      Transformation *childTransformation = dynamic_cast<Transformation *>(c.second);
      if (childTransformation)
        childTransformations.push_back(childTransformation);
    }
  }
  return childTransformations;
}

void Transformation::_recomputeTransformationMatrix()
{
  this->_transformationMatrix = glm::mat4x4();
  this->_transformationMatrix = glm::translate(this->_transformationMatrix, this->_absoluteTranslation);
  this->_transformationMatrix = glm::rotate(this->_transformationMatrix, glm::degrees(this->_absoluteRotation.x),
                                            glm::vec3(1.0f, 0.0f, 0.0f));
  this->_transformationMatrix = glm::rotate(this->_transformationMatrix, glm::degrees(this->_absoluteRotation.y),
                                            glm::vec3(0.0f, 1.0f, 0.0f));
  this->_transformationMatrix = glm::rotate(this->_transformationMatrix, glm::degrees(this->_absoluteRotation.z),
                                            glm::vec3(0.0f, 0.0f, 1.0f));
  this->_transformationMatrix = glm::scale(this->_transformationMatrix, this->_absoluteScaling);
}

} // namespace leo
