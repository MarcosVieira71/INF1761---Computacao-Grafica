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

ParticleEmitterPtr ParticleEmitter::Make(float radius) {
    return std::make_shared<ParticleEmitter>(radius);
}

ParticleEmitter::ParticleEmitter(float radius)
: vao_(0), vbo_(0), radius_(radius)
{
    float point[3] = {0.0f, 0.0f, 0.0f};
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParticleEmitter::~ParticleEmitter() {
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}

void ParticleEmitter::Draw(StatePtr st)
{
    ShaderPtr shd = st->GetShader();
    shd->SetUniform("radius", radius_);
    float t = float(glfwGetTime());
    shd->SetUniform("time", t);
    shd->SetUniform("particleSize", 0.01f);
    glm::mat4 proj = st->GetCamera()->GetProjMatrix();
    shd->SetUniform("Proj", proj);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(vao_);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
