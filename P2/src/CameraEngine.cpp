#include "../include/CameraEngine.h"

#include <glm/gtc/matrix_transform.hpp>

CameraEngine::CameraEngine(const Camera3DPtr& cam, const NodePtr& observer, const NodePtr& observable, const glm::vec3& offset)
: m_camera(cam), m_observer(observer), m_observable(observable), m_offset(offset)
{
}

CameraEnginePtr CameraEngine::Make(const Camera3DPtr& cam, const NodePtr& observer, const NodePtr& observable, const glm::vec3& offset)
{
    return std::make_shared<CameraEngine>(cam, observer, observable, offset);
}

void CameraEngine::Update(float dt)
{
    if (!m_camera || !m_observer || !m_observable)
        return;

    glm::mat4 matObserver = m_observer->GetModelMatrix();
    glm::vec3 posObserver = glm::vec3(matObserver[3]);

    glm::mat4 matObservable = m_observable->GetModelMatrix();
    glm::vec3 posObservable = glm::vec3(matObservable[3]);

    glm::vec3 dir = posObservable - posObserver;
    float dist = glm::length(dir);
    if (dist < 1e-6f) {
        glm::vec3 eye = posObserver + m_offset;
        m_camera->SetEye(eye.x, eye.y, eye.z);
    } else {
        dir = glm::normalize(dir);

        float sx = glm::length(glm::vec3(matObserver[0]));
        float sy = glm::length(glm::vec3(matObserver[1]));
        float sz = glm::length(glm::vec3(matObserver[2]));
        float radius = std::max(std::max(sx, sy), sz);

        float above = radius * 1.05f + 0.3f; 
        glm::vec3 eye = posObserver + dir * above;
        m_camera->SetEye(eye.x, eye.y, eye.z);
    }

    m_camera->SetCenter(posObservable.x, posObservable.y, posObservable.z);

}
void CameraEngine::setObserver(const NodePtr& observer)
{ m_observer = observer; }

void CameraEngine::setObservable(const NodePtr& observable) 
{ m_observable = observable; }
