#include <memory>

#pragma once

class Quad;
using QuadPtr = std::shared_ptr<Quad>; 
class State;
using StatePtr = std::shared_ptr<State>;

#include "Shape.h"

class Quad : public Shape {
  unsigned int m_vao;
  unsigned int m_nind; // number of incident vertices
protected:
  Quad (int nx, int ny);
public:
  static QuadPtr Make (int nx=1, int ny=1);
  virtual ~Quad ();
  virtual void Draw (StatePtr st);
};