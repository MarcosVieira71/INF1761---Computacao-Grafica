#pragma once

#include "Engine.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <map>

class AstralBody;
using AstralBodyPtr = std::shared_ptr<AstralBody>;

class Orbit;
using OrbitPtr = std::shared_ptr<Orbit>;

class AstralEngine;
using AstralEnginePtr = std::shared_ptr<AstralEngine>;

class AstralEngine : public Engine {

    std::map<TransformPtr, float> trfAstroMap;
    std::map<TransformPtr, float> trfOrbitMap;

public:
    AstralEngine();
    void Update(float dt) override;
    void addOrbit(OrbitPtr orb, float speed);
    void addAxis(AstralBodyPtr astro, float speed);
    static AstralEnginePtr Make();

};
