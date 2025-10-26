#include <memory>
#pragma once

class Shape;
using ShapePtr = std::shared_ptr<Shape>; 

class State;
using StatePtr = std::shared_ptr<State>; 

class Shape {
protected:
  Shape () {}
public:
  enum LOC {
    COORD=0,
    NORMAL,
    TANGENT,
    TEXCOORD
  };
  virtual ~Shape () {}
  virtual void Draw (StatePtr st) = 0;
};

