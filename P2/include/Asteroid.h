#pragma once

#include "AstralBody.h"

class Asteroid : public AstralBody
{
public:
    Asteroid(const glm::vec3& pos, const glm::vec3& scale, ShapePtr shape = Sphere::Make(8,8));

    static std::shared_ptr<Asteroid> Make(const glm::vec3& pos, const glm::vec3& scale, ShapePtr shape = Sphere::Make(8,8));
};
