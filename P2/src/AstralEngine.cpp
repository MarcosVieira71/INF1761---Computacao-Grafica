#include "AstralEngine.h"

#include "Orbit.h"
#include "AstralBody.h"
AstralEngine::AstralEngine() = default;

AstralEnginePtr AstralEngine::Make()
{
    return std::make_shared<AstralEngine>();
}

void AstralEngine::Update(float dt)
{
    for(auto [trf, speed] : trfAstroMap)
    {
        trf->Rotate(dt * speed, 0, 1, 0);
    }
    
    for(auto [trf, speed] : trfOrbitMap)
    {
        trf->Rotate(dt * speed, 0, 1, 0);
    }


    //Usamos isso para fazer tidal lock;
    if (m_earth && m_moon)
    {
        glm::vec3 posEarth = glm::vec3(m_earth->GetModelMatrix()[3]);
        glm::vec3 posMoon  = glm::vec3(m_moon->GetModelMatrix()[3]);

        glm::vec3 targetDir = glm::normalize(posEarth - posMoon);
        glm::mat4 M = m_moon->GetModelMatrix();
        glm::vec3 forward = glm::normalize(-glm::vec3(M[2]));

        glm::vec3 axis = glm::cross(forward, targetDir);
        float len = glm::length(axis);
        if (len > 1e-6f)
        {
            axis /= len;
            float angle = acos(glm::clamp(glm::dot(forward, targetDir), -1.0f, 1.0f));
            m_moon->GetTransform()->Rotate(angle, axis.x, axis.y, axis.z);
        }
    }
}

void AstralEngine::addOrbit(OrbitPtr orb, float speed)
{
    auto& t = orb->GetTransform();
    trfOrbitMap[t] = speed;
}

void AstralEngine::addAxis(AstralBodyPtr astro, float speed)
{
    auto& t = astro->GetTransform();
    trfAstroMap[t] = speed;
}

void AstralEngine::setEarth(NodePtr earth)
{
    m_earth = earth;
}

void AstralEngine::setMoon(NodePtr moon)
{
    m_moon = moon;
}