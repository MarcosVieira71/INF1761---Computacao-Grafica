#pragma once

#include <map>
#include <string>
#include <tuple>

#include "Sphere.h"
#include "Orbit.h"
#include "AstralBody.h"

namespace PlanetBuilder {
    ShapePtr CreateSphere();

    void BuildPlanets(std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>> &ptr_map,
                      const ShapePtr &sphere,
                      OrbitPtr orbSun,
                      AstralBodyPtr astroSun);

    void BuildMoonAndRings(std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>> &ptr_map,
                           const ShapePtr &sphere,
                           const ShaderPtr &shaderNormal);
}
