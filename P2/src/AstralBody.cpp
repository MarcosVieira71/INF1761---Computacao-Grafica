#include "AstralBody.h"

#include "Orbit.h"
#include "Transform.h"
#include "Appearance.h"

AstralBodyPtr AstralBody::Make(const glm::vec3& pos, const glm::vec3& scale, const std::vector<AppearancePtr>& apps, ShapePtr shape)
{
    return std::make_shared<AstralBody>(pos, scale, apps, shape);
}

AstralBody::AstralBody(const glm::vec3& pos, const glm::vec3& scale, const std::vector<AppearancePtr>& apps, ShapePtr shape)
{
    auto t = Transform::Make();
    t->Translate(pos.x, pos.y, pos.z);
    t->Scale(scale.x, scale.y, scale.z);
    AddShape(shape);
    SetTransform(t);
    for(auto app : apps){
        AddAppearance(app);
    }
}

AstralBody::~AstralBody() = default;

void AstralBody::setup(OrbitPtr astro)
{
    AddNode(astro);
}
