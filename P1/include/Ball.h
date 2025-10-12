#pragma once

#include <glm/glm.hpp>

#include <memory>

class Node;
using NodePtr = std::shared_ptr<Node>;

struct Ball
{

    glm::vec2 position;
    glm::vec2 last_position;
    glm::vec2 acceleration;
    float radius;

    Ball(glm::vec2 pos0, float r);
};

