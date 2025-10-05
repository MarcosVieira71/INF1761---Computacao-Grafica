#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include "Transform.h"

#include "State.h"

#include <glm/gtc/matrix_transform.hpp>


TransformPtr Transform::Make ()
{
  return TransformPtr(new Transform());
}

Transform::Transform ()
: m_mat(1.0f)
{
}
Transform::~Transform ()
{
}
void Transform::LoadIdentity ()
{
  m_mat = glm::mat4(1.0f);
}
void Transform::MultMatrix (const glm::mat4 mat)
{
  m_mat *= mat;
}
void Transform::Translate (float x, float y, float z)
{
  m_mat = glm::translate(m_mat,glm::vec3(x,y,z));
}
void Transform::Scale (float x, float y, float z)
{
  m_mat = glm::scale(m_mat,glm::vec3(x,y,z));
}
void Transform::Rotate (float angle, float x, float y, float z)
{
  m_mat = glm::rotate(m_mat,glm::radians(angle),glm::vec3(x,y,z));
}
const glm::mat4& Transform::GetMatrix() const
{
  return m_mat;
}

void Transform::Load (StatePtr st) const
{
  st->PushMatrix();
  st->MultMatrix(m_mat);
}

void Transform::Unload (StatePtr st) const
{
  st->PopMatrix();
}
