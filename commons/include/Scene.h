#include <memory>
#pragma once

#include "Node.h"

class Scene;
using ScenePtr = std::shared_ptr<Scene>; 

class Camera;
using CameraPtr = std::shared_ptr<Camera>; 

class Engine;
using EnginePtr = std::shared_ptr<Engine>; 

class Scene : public Node
{
  NodePtr m_root;
  std::vector<EnginePtr> m_engines;
protected:
  Scene (NodePtr root);
public:
  static ScenePtr Make (NodePtr root);
  ~Scene ();
  NodePtr GetRoot () const;
  void AddEngine (EnginePtr engine);
  void Update (float dt) const;
  void Render (CameraPtr camera);
};
