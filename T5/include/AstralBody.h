#pragma once

#include "Node.h"

#include <glm/glm.hpp>
#include <memory>

class AstralBody;
using AstralBodyPtr = std::shared_ptr<AstralBody>;

class Orbit;
using OrbitPtr = std::shared_ptr<Orbit>;

class AstralBody : public Node
{   
public:

    AstralBody(const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app);
    ~AstralBody();

    static AstralBodyPtr Make(const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app);

};
