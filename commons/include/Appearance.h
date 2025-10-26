#include <memory>
#pragma once

class Appearance;
using  AppearancePtr = std::shared_ptr<Appearance>;


#include "State.h"

class Appearance {
public:
  virtual ~Appearance () {}
  virtual void Load (StatePtr st) = 0;
  virtual void Unload (StatePtr ) { }
};

