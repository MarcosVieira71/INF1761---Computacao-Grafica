#ifdef __APPLE__
  #include <OpenGL/gl3.h>
#else
  #include <GL/glew.h>
#endif

#include "ParticleEmitter.h"
#include "State.h"
#include "Camera.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

ParticleEmitterPtr ParticleEmitter::Make(float radius, float size) {
    return std::make_shared<ParticleEmitter>(radius, size);
}

ParticleEmitter::ParticleEmitter(float radius, float size)
: _vao(0), _vbo(0), _radius(radius), _size(size)
{
    float point[3] = {0.0f, 0.0f, 0.0f};
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParticleEmitter::~ParticleEmitter() {
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
}

void ParticleEmitter::Draw(StatePtr st)
{
    ShaderPtr shd = st->GetShader();
    shd->SetUniform("radius", _radius);
    float t = float(glfwGetTime());
    shd->SetUniform("time", t);
    shd->SetUniform("particleSize", _size);
    glm::mat4 proj = st->GetCamera()->GetProjMatrix();
    shd->SetUniform("Proj", proj);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(_vao);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
