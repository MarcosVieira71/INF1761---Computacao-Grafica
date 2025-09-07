#pragma once
#include <memory>

class Polygon;
using PolygonPtr = std::shared_ptr<Polygon>; 

#include "shape.h"

class Polygon : public Shape {
  unsigned int m_vao;
protected:
  Polygon();
public:
  static PolygonPtr Make ();
  virtual ~Polygon ();
  virtual void Draw ();
};
