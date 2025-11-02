#pragma once

#include "Engine.h"
#include "Camera3D.h"
#include "Node.h"

#include <memory>

class CameraEngine;
using CameraEnginePtr = std::shared_ptr<CameraEngine>;

class CameraEngine : public Engine {
    Camera3DPtr m_camera;
    NodePtr m_observer;
    NodePtr m_observable;
    glm::vec3 m_offset;
public:
    CameraEngine(const Camera3DPtr& cam, const NodePtr& observer, const NodePtr& observable, const glm::vec3& offset = glm::vec3(0.0f,5.0f,10.0f));
    virtual ~CameraEngine() = default;
    void Update(float dt) override;
    void setObserver(const NodePtr& observer);
    void setObservable(const NodePtr& observable);
    static CameraEnginePtr Make(const Camera3DPtr& cam, const NodePtr& observer, const NodePtr& observable, const glm::vec3& offset = glm::vec3(0.0f,5.0f,10.0f));
};
