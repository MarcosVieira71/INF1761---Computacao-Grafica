#include <memory>
#pragma once

class Color;
using ColorPtr = std::shared_ptr<Color>; 

#include "Appearance.h"
#include <glm/glm.hpp>

class Color : public Appearance {
  glm::vec4 m_color; 
protected:
  Color (float r, float g, float b, float a);
public:
  static ColorPtr Make (float r, float g, float b, float a=1.0f);
  virtual ~Color ();
  virtual void Load (StatePtr st);
};
