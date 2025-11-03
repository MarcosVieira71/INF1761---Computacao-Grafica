#pragma once

#include <memory>
class Ring;
using RingPtr = std::shared_ptr<Ring>;

#include "Shape.h"

class Ring : public Shape {
private:
  unsigned int m_vao;
  unsigned int m_vbo;
  unsigned int m_nbo;
  unsigned int m_tbo;
  unsigned int m_vertexNum;

protected:
  Ring(float innerRadius, float outerRadius, std::size_t segments);

public:
  static RingPtr Make(float innerRadius, float outerRadius, std::size_t segments);
  virtual ~Ring();
  virtual void Draw(StatePtr st) override;
};
