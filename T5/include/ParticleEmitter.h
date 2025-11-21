#pragma once

#include "Shape.h"
#include <memory>

class ParticleEmitter;
using ParticleEmitterPtr = std::shared_ptr<ParticleEmitter>;

class ParticleEmitter : public Shape {
public:
    static ParticleEmitterPtr Make(float radius = 1.25f);
    ParticleEmitter(float radius = 1.25f);
    ~ParticleEmitter();
    virtual void Draw(StatePtr st) override;
private:
    unsigned int vao_, vbo_;
    float radius_;
};
