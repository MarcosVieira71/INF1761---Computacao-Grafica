#include "SolarSystemEngine.h"

#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

SolarSystemEngine::SolarSystemEngine(TransformPtr sunTrf, TransformPtr earthTrf, TransformPtr moonTrf)
        : m_sunTrf(sunTrf), m_earthTrf(earthTrf), m_moonTrf(moonTrf)
    {}

void SolarSystemEngine::Update(float dt) {
    m_time += dt;

    m_earthTrf->LoadIdentity();            
    m_earthTrf->Rotate(m_time * m_earthSpeedSun, 0, 0, 1);

    m_moonTrf->LoadIdentity();
    m_moonTrf->Rotate(m_time * m_moonSpeedEarth, 0, 0, 1); 
}


EnginePtr SolarSystemEngine::Make(TransformPtr sunTrf, TransformPtr earthTrf, TransformPtr moonTrf)
{
    return EnginePtr(new SolarSystemEngine(sunTrf, earthTrf, moonTrf));
}
