#include <memory>
#pragma once

class State;
using StatePtr = std::shared_ptr<State>; 

class Camera;
class Shader;

using ShaderPtr = std::shared_ptr<Shader>;
using CameraPtr = std::shared_ptr<Camera>;

#include <glm/glm.hpp>
#include <string>
#include <vector>

class State : public std::enable_shared_from_this<State> {
  CameraPtr m_camera;
  std::vector<ShaderPtr> m_shader;
  std::vector<glm::mat4> m_stack;
protected:
  State (CameraPtr camera);
public:
  static StatePtr Make (CameraPtr camera);
  virtual ~State ();
  void PushShader (ShaderPtr shd);
  void PopShader ();
  void PushMatrix ();
  void PopMatrix ();
  void LoadMatrix (const glm::mat4& mtx);
  void MultMatrix (const glm::mat4& mtx);
  const glm::mat4& GetCurrentMatrix () const;
  ShaderPtr GetShader () const;
  CameraPtr GetCamera () const;
  void LoadMatrices ();
};
