#pragma once

#include <model/icomponent.hpp>

namespace leo
{

class Texture;

class IBL : public IComponent
{
public:
    IBL(Texture &texture);

public:
    const Texture &getTexture() const;

public:
    virtual ComponentType getTypeId() const override
    {
        return ComponentType::IBL_MAP;
    }

public:
    Texture &_texture;
};

} // namespace leo
