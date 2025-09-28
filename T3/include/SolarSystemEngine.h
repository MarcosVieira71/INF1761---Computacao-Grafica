#pragma once

#include "Engine.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class SolarSystemEngine : public Engine {
    float m_earthSpeedSun = 10.f;
    float m_moonSpeedEarth = m_earthSpeedSun / 2;
    TransformPtr m_earthTrf;
    TransformPtr m_moonTrf;
    TransformPtr m_sunTrf;
    float m_time = 0.0f;

protected:
    SolarSystemEngine(TransformPtr sunPtr, TransformPtr earthTrf, TransformPtr moonTrf);

public:
    void Update(float dt) override;
    static EnginePtr Make(TransformPtr sunTrf, TransformPtr earthTrf, TransformPtr moonTrf);
};
