#include "AstralBody.h"

#include "Orbit.h"
#include "Transform.h"
#include "Appearance.h"
#include "Disk.h"
#include "Ring.h"
#include "Color.h"
#include "Texture.h"
#include "Emissive.h"
#include "Node.h"

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

void AstralBody::AddRing(const std::string& texturePath, float relativeScale, std::size_t segments)
{

    float outer = relativeScale;
    float inner = relativeScale * 0.35f; 
    ShapePtr ringShape = Ring::Make(inner, outer, segments);
    AppearancePtr ringApp = Texture::Make("decal", texturePath);

    AppearancePtr ringEmissive = Emissive::Make(0.6f, 0.6f, 0.6f);

    auto trf = Transform::Make();
    trf->Rotate(88.9f, 1.0f, 0.0f, 0.0f);
    trf->Scale(relativeScale, relativeScale, relativeScale);

    auto ringNode = Node::Builder()
                        .WithTransform(trf)
                        .AddAppearance(ringApp)
                        .AddAppearance(ringEmissive)
                        .AddAppearance(Color::Make(1.f, 1.f, 1.f))
                        .AddShape(ringShape)
                        .Build();

    AddNode(ringNode);
}
