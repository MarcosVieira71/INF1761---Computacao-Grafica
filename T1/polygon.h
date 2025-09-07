#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>

class Polygon;
using PolygonPtr = std::shared_ptr<Polygon>; 

#include "shape.h"

class Polygon : public Shape {
private:
  void setupVAO();
  unsigned int m_vao;
  unsigned int m_vbo[2];
  unsigned int m_ebo;
  unsigned int m_vertexNum;
  std::vector<unsigned int> m_idxs;

protected:
Polygon(const std::vector<glm::vec2>& vertices,
        const std::vector<glm::vec3>& colors,
        const std::vector<unsigned int>& idxs);
public:
  static PolygonPtr Make (const std::vector<glm::vec2>& vertices, 
                          const std::vector<glm::vec3>& colors, 
                          const std::vector<unsigned int>& idxs);
  virtual ~Polygon ();
  virtual void Draw ();
};
