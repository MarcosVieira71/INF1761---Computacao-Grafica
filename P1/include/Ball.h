#pragma once

#include <glm/glm.hpp>

#include <memory>

class Node;
using NodePtr = std::shared_ptr<Node>;

struct Ball
{

    glm::vec3 position;
    glm::vec3 last_position;
    glm::vec3 acceleration;
    float radius;

    Ball(glm::vec3 pos0, float r);
};

