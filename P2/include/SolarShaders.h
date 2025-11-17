#pragma once

#include <memory>
#include "Light.h"
#include "Shader.h"

namespace SolarShaders {
    struct ShaderSet {
        ShaderPtr main;
        ShaderPtr normal;
        ShaderPtr sky;
        ShaderPtr shadow;
    };

    LightPtr CreateLight();
    ShaderSet CreateShaders(const LightPtr &light);
}
