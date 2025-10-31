#include "AstralBody.h"

#include "Orbit.h"
#include "Sphere.h"
#include "Transform.h"
#include "Appearance.h"

AstralBodyPtr AstralBody::Make(const glm::vec3& pos, const glm::vec3& scale, const std::vector<AppearancePtr>& apps)
{
    return std::make_shared<AstralBody>(pos, scale, apps);
}

AstralBody::AstralBody(const glm::vec3& pos, const glm::vec3& scale, const std::vector<AppearancePtr>& apps)
{
    auto t = Transform::Make();
    t->Translate(pos.x, pos.y, pos.z);
    t->Scale(scale.x, scale.y, scale.z);
    AddShape(Sphere::Make(64, 64));
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
