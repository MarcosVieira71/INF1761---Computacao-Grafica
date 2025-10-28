#include "AstralBody.h"

#include "Orbit.h"
#include "Sphere.h"
#include "Transform.h"
#include "Appearance.h"

AstralBodyPtr AstralBody::Make(const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app)
{
    return std::make_shared<AstralBody>(pos, scale, app);
}

AstralBody::AstralBody(const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app)
{
    auto t = Transform::Make();
    t->Translate(pos.x, pos.y, pos.z);
    t->Scale(scale.x, scale.y, scale.z);
    AddShape(Sphere::Make(64, 64));
    SetTransform(t);
    AddAppearance(app);
}

AstralBody::~AstralBody() = default;