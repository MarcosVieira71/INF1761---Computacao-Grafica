#include "Base.h"

#include "Transform.h"
#include "Color.h"
#include "Cylinder.h"
#include "Orbit.h"

BasePtr Base::Make(float radius, float height, const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app)
{
    return std::make_shared<Base>(radius, height, pos, scale, app);
}

Base::Base(float radius, float height, const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app) : _pivot(Pivot::Make(radius, height, scale, app))
{
    auto t = Transform::Make();
    t->Translate(pos.x, pos.y, pos.z);
    AddAppearance(Color::Make(1.0f, 1.0f, 1.0f));
    SetTransform(t);
}

void Base::setup(OrbitPtr orb)
{
    AddNode(_pivot);
    AddNode(orb);
}

Base::~Base() = default;

Base::Pivot::PivotPtr Base::Pivot::Make(float radius, float height, const glm::vec3& scale, AppearancePtr app)
{
    return std::make_shared<Pivot>(radius, height, scale, app);
}

Base::Pivot::Pivot(float radius, float height, const glm::vec3& scale, AppearancePtr app)
{
    auto t = Transform::Make();
    t->Scale(scale.x, scale.y, scale.z);
    AddShape(Cylinder::Make(radius, height, 64, true));
    AddAppearance(app);
    SetTransform(t);
}

Base::Pivot::~Pivot() = default;
