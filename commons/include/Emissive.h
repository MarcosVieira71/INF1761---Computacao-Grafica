#pragma once

#include "Appearance.h"
#include <glm/glm.hpp>

class Emissive;
using EmissivePtr = std::shared_ptr<Emissive>;

class Emissive : public Appearance {
private:
    glm::vec3 m_emissionColor;

public:
    static EmissivePtr Make(float r, float g, float b);

    Emissive(float r, float g, float b);
    virtual ~Emissive();

    void SetColor(const glm::vec3& c);
    glm::vec3 GetColor() const;

    virtual void Load(StatePtr st) override;
    virtual void Unload(StatePtr st) override;
};
