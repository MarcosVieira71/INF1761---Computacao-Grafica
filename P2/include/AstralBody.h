#pragma once

#include "Node.h"
#include "Sphere.h"


#include <glm/glm.hpp>
#include <memory>

class AstralBody;
using AstralBodyPtr = std::shared_ptr<AstralBody>;

class Orbit;
using OrbitPtr = std::shared_ptr<Orbit>;

class AstralBody : public Node
{   
public:

    AstralBody(const glm::vec3& pos, const glm::vec3& scale, const std::vector<AppearancePtr>& apps, ShapePtr shape = Sphere::Make(64,64));
    ~AstralBody();

    static AstralBodyPtr Make(const glm::vec3& pos, const glm::vec3& scale, const std::vector<AppearancePtr>& apps, ShapePtr shape = Sphere::Make(64,64));

    void setup(OrbitPtr astro);

    void AddRing(const std::string& texturePath, float relativeScale = 2.0f, std::size_t segments = 128);

};
