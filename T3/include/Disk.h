#include <memory>
class Disk;
using DiskPtr = std::shared_ptr<Disk>; 
#pragma once

#include "Shape.h"

class Disk : public Shape {
  unsigned int m_vao;
protected:
  Disk ();
public:
  static DiskPtr Make ();
  virtual ~Disk ();
  virtual void Draw ();
};