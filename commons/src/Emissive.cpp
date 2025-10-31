#include "Emissive.h"
#include "Shader.h"

EmissivePtr Emissive::Make(float r, float g, float b) {
    return EmissivePtr(new Emissive(r,g,b));
}

Emissive::Emissive(float r, float g, float b)
    : m_emissionColor(r,g,b)
{}

Emissive::~Emissive() {}

void Emissive::SetColor(const glm::vec3& c) 
{ 
    m_emissionColor = c; 
}

glm::vec3 Emissive::GetColor() const 
{ 
    return m_emissionColor; 
}

void Emissive::Load(StatePtr st)
 {
    ShaderPtr shd = st->GetShader();
    shd->SetUniform("isEmissive", 1);
    shd->SetUniform("emissionColor", m_emissionColor);
}

void Emissive::Unload(StatePtr st)
{
    ShaderPtr shd = st->GetShader();
    shd->SetUniform("isEmissive", 0);
}
