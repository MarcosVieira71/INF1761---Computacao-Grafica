#pragma once

#include <memory>

class Cylinder;
using CylinderPtr = std::shared_ptr<Cylinder>;

#include "Shape.h"

class Cylinder : public Shape {
  unsigned int m_vao;
  unsigned int m_vbo;
  unsigned int m_nbo;
  unsigned int m_tbo;
  unsigned int m_vertexNum;
protected:
  Cylinder(float radius, float height, int segments, bool caps=true);
public:
  static CylinderPtr Make(float radius, float height, int segments, bool caps=true);
  virtual ~Cylinder();
  virtual void Draw(StatePtr st) override;
};
