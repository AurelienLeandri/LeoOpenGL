#pragma once

#include <model/component.hpp>
#include <model/components/point-light.hpp>
#include <model/components/direction-light.hpp>
#include <map>

namespace leo {
  namespace model {

    class Globals : public Component {
      public:
        Globals();
        Globals(const Globals &other);
        const Globals &operator=(const Globals &other);

      private:
      std::map<stringID, PointLight> pointLights;
      std::map<stringID, DirectionLight> directionLights;

    };

  } // namespace leo
} // namespace model
