#include "SolarSystemEngine.h"

#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

SolarSystemEngine::SolarSystemEngine(TransformPtr earthOrbitTrf, TransformPtr moonOrbitTrf)
        : m_earthOrbitTrf(earthOrbitTrf), m_moonOrbitTrf(moonOrbitTrf)
    {}

void SolarSystemEngine::Update(float dt)
{
    m_earthOrbitTrf->Rotate(dt * m_earthSpeedSun, 0, 0, 1);
    m_moonOrbitTrf->Rotate(dt * m_moonSpeedEarth, 0, 0, 1);
}


EnginePtr SolarSystemEngine::Make(TransformPtr earthOrbitTrf, TransformPtr moonOrbitTrf)
{
    return EnginePtr(new SolarSystemEngine(earthOrbitTrf, moonOrbitTrf));
}
