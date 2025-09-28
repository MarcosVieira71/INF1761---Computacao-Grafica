#include <memory>
#pragma once
class Camera2D;
using Camera2DPtr = std::shared_ptr<Camera2D>; 

class State;
using StatePtr = std::shared_ptr<State>; 


#include "Camera.h"
#include <glm/glm.hpp>

class Camera2D : public Camera 
{
  float m_xmin, m_xmax, m_ymin, m_ymax;
protected:
  Camera2D (float xmin, float xmax, float ymin, float ymax);
public:
  static Camera2DPtr Make (float xxmin, float xmax, float ymin, float ymax);
  virtual ~Camera2D ();
  virtual glm::mat4 GetProjMatrix () const;
  virtual glm::mat4 GetViewMatrix () const;
  virtual void Load (StatePtr st) const;
};
