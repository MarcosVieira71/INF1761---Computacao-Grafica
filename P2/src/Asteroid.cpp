#include "Asteroid.h"

#include "Transform.h"

Asteroid::Asteroid(const glm::vec3& pos, const glm::vec3& scale, ShapePtr shape)
    : AstralBody(pos, scale, std::vector<AppearancePtr>{}, shape)
{
    //Todo asteroide deve estar ligado a um container de asteroides e ele sim deve resolver sua questão de aparencia
}

std::shared_ptr<Asteroid> Asteroid::Make(const glm::vec3& pos, const glm::vec3& scale, ShapePtr shape)
{
    return std::make_shared<Asteroid>(pos, scale, shape);
}
