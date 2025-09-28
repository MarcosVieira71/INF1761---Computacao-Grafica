#pragma once

#include "Engine.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class SolarSystemEngine : public Engine {
    float m_earthSpeedSun = 25.f;
    float m_moonSpeedEarth = m_earthSpeedSun * 2;
    TransformPtr m_earthOrbitTrf;
    TransformPtr m_moonOrbitTrf;
    TransformPtr m_sunTrf;
    float m_time = 0.0f;

protected:
    SolarSystemEngine(TransformPtr earthOrbitTrf, TransformPtr moonOrbitTrf);

public:
    void Update(float dt) override;
    static EnginePtr Make(TransformPtr earthOrbitTrf, TransformPtr moonOrbitTrf);
};
