#pragma once

#include <memory>

class Cube;
using CubePtr = std::shared_ptr<Cube>; 

#include "Shape.h"

class Cube : public Shape {
  unsigned int m_vao;
protected:
  Cube ();
public:
  static CubePtr Make ();
  virtual ~Cube ();
  virtual void Draw (StatePtr st);
};
