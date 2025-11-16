#pragma once

#include <memory>
#include "Light.h"
#include "Shader.h"

namespace SolarShaders {
    struct ShaderSet {
        ShaderPtr main;
        ShaderPtr normal;
        ShaderPtr sky;
    };

    LightPtr CreateLight();
    ShaderSet CreateShaders(const LightPtr &light);
}
