#include "SolarShaders.h"

#include "Light.h"
#include "Shader.h"

using namespace SolarShaders;

LightPtr SolarShaders::CreateLight()
{
    auto light = Light::Make(0.0f, 0.0f, 0.0f, 1.0f, "world");
    light->SetAmbient(0.1f, 0.1f, 0.1f);
    light->SetDiffuse(1.0f, 1.0f, 0.9f);
    light->SetSpecular(1.0f, 1.0f, 0.9f);
    return light;
}

SolarShaders::ShaderSet SolarShaders::CreateShaders(const LightPtr &light)
{
    ShaderSet s;
    s.main = Shader::Make(light, "world");
    s.main->AttachVertexShader("../shaders/vertex.glsl");
    s.main->AttachFragmentShader("../shaders/fragment.glsl");
    s.main->Link();

    s.normal = Shader::Make(light, "world");
    s.normal->AttachVertexShader("../shaders/vertex_normal.glsl");
    s.normal->AttachFragmentShader("../shaders/fragment_normal.glsl");
    s.normal->Link();

    s.sky = Shader::Make(light, "world");
    s.sky->AttachVertexShader("../shaders/sky_vertex.glsl");
    s.sky->AttachFragmentShader("../shaders/sky_fragment.glsl");
    s.sky->Link();

    s.shadow = Shader::Make(light, "world");
    s.shadow->AttachVertexShader("../shaders/shadow_vertex.glsl");
    s.shadow->AttachFragmentShader("../shaders/shadow_fragment.glsl");
    s.shadow->Link();
    
    return s;
}
