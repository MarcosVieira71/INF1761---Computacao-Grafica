#include "Color.h"

#ifdef _WIN32
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include "Shader.h"
#include "State.h"

ColorPtr Color::Make(float r, float g, float b, float a)
{
  return ColorPtr(new Color(r, g, b, a));
}

Color::Color(float r, float g, float b, float a)
    : m_color(r, g, b, a)
{
}
Color::~Color()
{
}

void Color::Load(StatePtr st)
{
  ShaderPtr shd = st->GetShader();
  shd->SetUniform("color", m_color);

  if (m_color.a < 1.0f)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
  }
}

void Color::Unload(StatePtr st)
{
  if (m_color.a < 1.0f)
  {
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
  }
}
