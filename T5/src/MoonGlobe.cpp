#include "MoonGlobe.h"
#include "Transform.h"
#include "Color.h"
#include "Sphere.h"
#include "Cylinder.h"

MoonGlobe::Globe::Globe(AppearancePtr glassApp)
{
    auto t = Transform::Make();
    t->Translate(0.0f, 0.75f, 0.0f);
    t->Scale(0.5f, 0.5f, 0.5f);
    AddShape(Sphere::Make());
    AddAppearance(glassApp);
    SetTransform(t);
}

MoonGlobe::Globe::GlobePtr MoonGlobe::Globe::Make(AppearancePtr glassApp)
{
    return std::make_shared<Globe>(glassApp);
}

MoonGlobe::Globe::~Globe() = default;

MoonGlobe::Base::Base(float radius, float height, const glm::vec3& scale, AppearancePtr baseApp)
{
    auto t = Transform::Make();
    t->Translate(0.0f, -1.25f, 0.0f);
    t->Scale(scale.x, scale.y, scale.z);
    AddShape(Cylinder::Make(radius, height, 64, true));
    AddAppearance(baseApp);
    SetTransform(t);
}

MoonGlobe::Base::BasePtr MoonGlobe::Base::Make(float radius, float height, const glm::vec3& scale, AppearancePtr baseApp)
{
    return std::make_shared<Base>(radius, height, scale, baseApp);
}

MoonGlobe::Base::~Base() = default;

MoonGlobe::MoonGlobe(const glm::vec3& pos)
{
    auto t = Transform::Make();
    t->Translate(pos);
    AddAppearance(Color::Make(1.0f, 1.0f, 1.0f));
    SetTransform(t);
}

MoonGlobePtr MoonGlobe::Make(const glm::vec3& pos, AppearancePtr baseApp,
                              AppearancePtr glassApp)
{
    auto globeNode = std::make_shared<MoonGlobe>(pos);

    auto base = Base::Make(1.0f, 0.5f, {0.5f, 0.5f, 0.5f}, baseApp);  
    auto globe = Globe::Make(glassApp);
    base->AddNode(globe);
    globeNode->AddNode(base);

    return globeNode;
}

MoonGlobe::~MoonGlobe() = default;
