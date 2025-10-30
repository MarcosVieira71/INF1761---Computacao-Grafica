#include "Base.h"

#include "Transform.h"
#include "Color.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Orbit.h"

BasePtr Base::Make(float radius, float height, const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app)
{
    auto t = Transform::Make();
    t->Scale(0.5f, 0.5f, 0.5f);
    t->Translate(0.0f, -1.0f, 0.0f);
    auto c = Node::Builder().AddShape(Cube::Make()).WithTransform(t).Build();
    auto base = std::make_shared<Base>(radius, height, pos, scale, app);
    
    base->AddNode(c);
    base->AddNode(base->_pivot);

    return base;
}

Base::Base(float radius, float height, const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app) : _pivot(Pivot::Make(radius, height, scale))
{
    auto t = Transform::Make();
    t->Translate(pos.x, pos.y, pos.z);
    AddAppearance(Color::Make(1.0f, 1.0f, 1.0f));
    AddAppearance(app);
    SetTransform(t);
}

void Base::setup(NodePtr orb)
{
    AddNode(orb);
}

Base::~Base() = default;

Base::Pivot::PivotPtr Base::Pivot::Make(float radius, float height, const glm::vec3& scale)
{
    return std::make_shared<Pivot>(radius, height, scale);
}

Base::Pivot::Pivot(float radius, float height, const glm::vec3& scale)
{
    auto t = Transform::Make();
    t->Scale(scale.x, scale.y, scale.z);
    AddShape(Cylinder::Make(radius, height, 64, true));
    SetTransform(t);
}

Base::Pivot::~Pivot() = default;