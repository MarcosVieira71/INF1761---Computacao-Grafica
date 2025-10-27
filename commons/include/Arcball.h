#pragma once

#include <memory>

#include <glm/glm.hpp>

class Arcball;
using ArcballPtr = std::shared_ptr<Arcball>; 


class Arcball {
  int m_x0, m_y0;
  float m_distance;
  glm::mat4 m_mat; // kept for backward compatibility; will be computed from m_rot and m_distance
  glm::mat4 m_rot; // accumulated rotation
protected:
  Arcball (float distance);
public:
  static ArcballPtr Make (float distance);
  void InitMouseMotion (int x, int y);
  void AccumulateMouseMotion (int x, int y);
  const glm::mat4& GetMatrix () const;

};

