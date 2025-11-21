#include "Orbit.h"

#include "Transform.h"
#include "AstralBody.h"

OrbitPtr Orbit::Make()
{
    return std::make_shared<Orbit>();
}

Orbit::Orbit()
{
    TransformPtr t = Transform::Make();
    SetTransform(t);
}

Orbit::~Orbit() = default;

void Orbit::setup(AstralBodyPtr astro)
{
    AddNode(astro);
}
