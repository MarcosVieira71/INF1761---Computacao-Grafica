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