#include <memory>
#pragma once

class Engine;
using EnginePtr = std::shared_ptr<Engine>; 


class Engine {
  protected:
  Engine () {}
  public:
  virtual ~Engine () {}
  virtual void Update (float dt) = 0;  // to update the world
};
