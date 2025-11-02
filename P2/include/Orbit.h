#pragma once

#include "Node.h"
#include <memory>


class Orbit;
using OrbitPtr = std::shared_ptr<Orbit>;

class AstralBody;
using AstralBodyPtr = std::shared_ptr<AstralBody>;

class Orbit : public Node
{
public:

    static OrbitPtr Make();
    Orbit();
    void setup(AstralBodyPtr astral);

    ~Orbit();
};
