#include "AsteroidBelt.h"

#include "Sphere.h"
#include "Texture.h"
#include "Emissive.h"
#include "Orbit.h"

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

AsteroidBelt::AsteroidBelt(AstralEnginePtr engine,
                           AstralBodyPtr sun,
                           std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>>& ptr_map,
                           ShaderPtr shaderNormal,
                           AppearancePtr nonEmissive,
                           int count,
                           unsigned int seed)
    : m_engine(engine), m_sun(sun), m_ptr_map(ptr_map), m_shaderNormal(shaderNormal), m_nonEmissive(nonEmissive), m_count(count), m_seed(seed)
{
}

void AsteroidBelt::Generate()
{
    AstralBodyPtr mars = std::get<1>(m_ptr_map.at("mars"));
    AstralBodyPtr jupiter = std::get<1>(m_ptr_map.at("jupiter"));

    glm::vec3 marsPos = glm::vec3(mars->GetTransform()->GetMatrix()[3]);
    glm::vec3 jupPos = glm::vec3(jupiter->GetTransform()->GetMatrix()[3]);

    float rMars = std::sqrt(marsPos.x * marsPos.x + marsPos.z * marsPos.z);
    float rJup = std::sqrt(jupPos.x * jupPos.x + jupPos.z * jupPos.z);

    float innerR = std::min(rMars, rJup) + 0.1f;
    float outerR = std::max(rMars, rJup) - 0.1f;
    if (outerR <= innerR) {
        innerR = rMars * 0.9f;
        outerR = rJup * 1.1f;
    }

    std::mt19937 rng(m_seed);
    std::uniform_real_distribution<float> U(0.0f, 1.0f);

    ShapePtr sphere = Sphere::Make(16, 16);

    auto tex1 = Texture::Make("decal", "../textures/asteroid.jpg");
    auto tex2 = Texture::Make("normalMap", "../textures/asteroid_normal.jpg");

    for (int a = 0; a < m_count; ++a)
    {
        float t = U(rng);
        float radius = innerR + t * (outerR - innerR);
        float angle = U(rng) * 2.0f * glm::pi<float>();
        float yoff = (U(rng) - 0.5f) * 0.05f * radius;

        glm::vec3 pos = {radius * std::cos(angle), yoff, radius * std::sin(angle)};
        float base = 0.01f + U(rng) * 0.04f;

        ShapePtr chosenShape;
        glm::vec3 scale;

        chosenShape = sphere;
        scale = glm::vec3(
            base * (0.3f + U(rng) * 2.2f),
            base * (0.2f + U(rng) * 2.6f),
            base * (0.4f + U(rng) * 2.0f)
        );

        pos += glm::vec3((U(rng) - 0.5f) * 0.02f * radius,
                         (U(rng) - 0.5f) * 0.02f * radius,
                         (U(rng) - 0.5f) * 0.02f * radius);

        std::vector<AppearancePtr> apps = { tex1, tex2, m_nonEmissive };
        auto asteroid = AstralBody::Make(pos, scale, apps, chosenShape);
        auto orb = Orbit::Make();
        orb->setup(asteroid);
        asteroid->SetShader(m_shaderNormal);

        float rx = U(rng) * 360.0f;
        float ry = U(rng) * 360.0f;
        float rz = U(rng) * 360.0f;
        asteroid->GetTransform()->Rotate(rx, 1.0f, 0.0f, 0.0f);
        asteroid->GetTransform()->Rotate(ry, 0.0f, 1.0f, 0.0f);
        asteroid->GetTransform()->Rotate(rz, 0.0f, 0.0f, 1.0f);

        m_sun->setup(orb);

        float spdOrbit = 5.0f + U(rng) * 10.0f;
        float spdAxis = 10.0f + U(rng) * 30.0f;
        m_engine->addOrbit(orb, spdOrbit);
        m_engine->addAxis(asteroid, spdAxis);
    }
}
