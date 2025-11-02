#pragma once

#include "AstralEngine.h"
#include "AstralBody.h"
#include "Orbit.h"
#include "Shader.h"

#include <map>
#include <string>
#include <memory>

class AsteroidBelt {
public:
    AsteroidBelt(AstralEnginePtr engine,
                 AstralBodyPtr sun,
                 std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>>& ptr_map,
                 ShaderPtr shaderNormal,
                 AppearancePtr nonEmissive,
                 int count = 300,
                 unsigned int seed = 123456);

    void Generate();

private:
    AstralEnginePtr m_engine;
    AstralBodyPtr m_sun;
    std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>>& m_ptr_map;
    ShaderPtr m_shaderNormal;
    AppearancePtr m_nonEmissive;
    int m_count;
    unsigned int m_seed;
};
