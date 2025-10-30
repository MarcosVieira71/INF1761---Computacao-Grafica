#include <memory>
#pragma once

class Transform;
using TransformPtr = std::shared_ptr<Transform>; 

class State;
using StatePtr = std::shared_ptr<State>; 

#include <glm/glm.hpp>


class Transform {
  glm::mat4 m_mat;
protected:
  Transform ();
public:
  static TransformPtr Make ();
  virtual ~Transform ();
  void LoadIdentity ();
  void MultMatrix (const glm::mat4 mat);
  void Translate (float x, float y, float z);
  void Scale (float x, float y, float z);
  void Rotate (float angle, float x, float y, float z);
  void Translate (const glm::vec3& pos);
  void Scale (const glm::vec3& scale);
  void Rotate (float angle, const glm::vec3& rotation);
  const glm::mat4& GetMatrix () const;
  void Load (StatePtr st) const;
  void Unload (StatePtr st) const;
};

