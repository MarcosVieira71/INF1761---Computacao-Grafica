#pragma once
#include "Node.h"
#include <memory>
#include <string>
#include "Shape.h"

class Obj;
using ObjPtr = std::shared_ptr<Obj>;

class Obj : public Shape {
  unsigned int m_vao;
  unsigned int m_count;
  unsigned int m_vbo[3]; 
protected:
  Obj();
public:
  static ObjPtr MakeFromFile(const std::string& filename);
  virtual ~Obj();
  virtual void Draw(StatePtr st);
};


NodePtr LoadObjNode(const std::string& filename);
