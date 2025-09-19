#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <vector>

class Circle;
using CirclePtr = std::shared_ptr<Circle>; 

#include "Shape.h"

class Circle : public Shape {
private:
  unsigned int m_vao;
  unsigned int m_vbo;
  unsigned int m_vertexNum;


protected:
Circle(float radius,
      std::size_t segments);
public:

  static CirclePtr Make (float radius,
                        std::size_t segments);
  virtual ~Circle ();
  virtual void Draw ();
};
