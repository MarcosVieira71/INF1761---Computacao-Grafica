#include <memory>
#pragma once

class Camera;
using CameraPtr = std::shared_ptr<Camera>; 

class State;
using StatePtr = std::shared_ptr<State>;

#include <glm/glm.hpp>


class Camera {
public:
  virtual glm::mat4 GetProjMatrix () const = 0;
  virtual glm::mat4 GetViewMatrix () const = 0;
  virtual void Load (StatePtr st) const = 0;
};

