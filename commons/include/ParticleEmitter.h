#pragma once

#include "Shape.h"
#include <memory>

class ParticleEmitter;
using ParticleEmitterPtr = std::shared_ptr<ParticleEmitter>;

class ParticleEmitter : public Shape {
public:
    static ParticleEmitterPtr Make(float radius = 1.25f, float size = 0.01f);
    ParticleEmitter(float radius = 1.25f, float size = 0.01f);
    ~ParticleEmitter();
    virtual void Draw(StatePtr st) override;
private:
    unsigned int _vao, _vbo;
    float _radius, _size;
};
