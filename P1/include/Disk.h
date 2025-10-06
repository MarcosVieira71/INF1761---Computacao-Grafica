#include <memory>
#pragma once

class Disk;
using DiskPtr = std::shared_ptr<Disk>; 

#include "Shape.h"

class Disk : public Shape {

private:
  unsigned int m_vao;
  unsigned int m_vbo;
  unsigned int m_tbo;
  unsigned int m_vertexNum;

protected:
  Disk(float radius, std::size_t segments);

  public:
  static DiskPtr Make(float radius, std::size_t segments);
  virtual ~Disk ();
  virtual void Draw(StatePtr st) override;
};