#include "Orbit.h"

#include "Transform.h"
#include "AstralBody.h"
#include "Color.h"

OrbitPtr Orbit::Make()
{
    return std::make_shared<Orbit>();
}

Orbit::Orbit()
{
    AddAppearance(Color::Make(1.0f, 1.0f, 1.0f));    
    TransformPtr t = Transform::Make();
    SetTransform(t);
}

Orbit::~Orbit() = default;

void Orbit::setup(AstralBodyPtr astro)
{
    AddNode(astro);
}
