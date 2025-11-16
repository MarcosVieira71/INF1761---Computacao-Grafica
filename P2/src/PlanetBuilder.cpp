#include "PlanetBuilder.h"

#include "Texture.h"
#include "Emissive.h"
#include "Orbit.h"
#include "AstralBody.h"
#include "Sphere.h"
#include "Texcube.h"

#include <random>
#include <array>

using namespace PlanetBuilder;

ShapePtr PlanetBuilder::CreateSphere()
{
    return Sphere::Make(32, 32);
}

void PlanetBuilder::BuildPlanets(std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>> &ptr_map,
                                 const ShapePtr &sphere,
                                 OrbitPtr orbSun,
                                 AstralBodyPtr astroSun)
{
    std::array<std::string, 8> names = {"mercury", "venus", "earth",
                                        "mars", "jupiter", "saturn", "uranus", "neptune"};

    std::array<glm::vec3, 8> scales = {
        glm::vec3{0.003f, 0.003f, 0.003f} * 25.0f,
        glm::vec3{0.008f, 0.008f, 0.008f} * 25.0f,
        glm::vec3{0.009f, -0.009f, 0.009f} * 25.0f,
        glm::vec3{0.005f, 0.005f, 0.005f} * 25.0f,
        glm::vec3{0.10f, 0.10f, 0.10f} * 5.0f,
        glm::vec3{0.08f, 0.08f, 0.08f} * 5.0f,
        glm::vec3{0.04f, 0.04f, 0.04f} * 10.0f,
        glm::vec3{0.03f, 0.03f, 0.03f} * 10.0f
    };

    int i = 0;
    int k = 0;
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> distSmall(0.1f, 0.3f);
    std::uniform_real_distribution<float> distLarge(0.3f, 0.8f);
    float accumulated = 1.0f;
    auto nonEmissive = Emissive::Make(0.f, 0.f, 0.f);

    for (const auto &planet : names)
    {
        i++;
        std::string texturePath = "../textures/" + planet + ".jpg";

        float baseDistance = 1.0f;
        float randomOffset = (i <= 4) ? distSmall(rng) : distLarge(rng);
        if (planet == "saturn")
        {
            randomOffset *= 3.0f;
        }

        accumulated += baseDistance + randomOffset;

        glm::vec3 pos = {accumulated, 0.f, 0.f};

        std::vector<AppearancePtr> apps = { Texture::Make("decal", texturePath), nonEmissive };

        if(planet == "earth")
            apps.push_back(Texture::Make("normalMap", "../textures/earth-normal.png" ));

        const glm::vec3 scale = scales[k];
        auto astro = AstralBody::Make(std::move(pos), std::move(scale), std::move(apps), sphere);
        auto orbit = Orbit::Make();

        ptr_map[planet] = {orbit, astro};
        orbit->setup(astro);
        astroSun->setup(orbit);
        k++;
    }
}

void PlanetBuilder::BuildMoonAndRings(std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>> &ptr_map,
                                      const ShapePtr &sphere,
                                      const ShaderPtr &shaderNormal)
{
    OrbitPtr orbMoon = Orbit::Make();
    AstralBodyPtr astroMoon = AstralBody::Make(
        {2.0f, 0.0f, 0.0f},
        {0.27f, 0.27f, 0.27f},
        {Texture::Make("decal", "../textures/moon.jpg"), Texture::Make("normalMap", "../textures/moon_normal.jpg"), Emissive::Make(0.f, 0.f, 0.f)},
        sphere);
    ptr_map["moon"] = {orbMoon, astroMoon};
    orbMoon->setup(astroMoon);

    auto& [orbitSaturn, astroSaturn] = ptr_map["saturn"];
    astroSaturn->AddRing("../textures/saturns_ring.png", 1.5f, 32);

    const auto &[orbit, astroEarth] = ptr_map["earth"];
    astroEarth->setup(orbMoon);
    astroEarth->SetShader(shaderNormal);
}
