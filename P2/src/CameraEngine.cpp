#include "../include/CameraEngine.h"

#include <glm/gtc/matrix_transform.hpp>

CameraEngine::CameraEngine(const Camera3DPtr& cam, const NodePtr& earth, const NodePtr& moon, const glm::vec3& offset)
: m_camera(cam), m_earth(earth), m_moon(moon), m_offset(offset)
{
}

CameraEnginePtr CameraEngine::Make(const Camera3DPtr& cam, const NodePtr& earth, const NodePtr& moon, const glm::vec3& offset)
{
    return std::make_shared<CameraEngine>(cam, earth, moon, offset);
}

void CameraEngine::Update(float dt)
{
    if (!m_camera || !m_earth || !m_moon)
        return;

    glm::mat4 matEarth = m_earth->GetModelMatrix();
    glm::vec3 posEarth = glm::vec3(matEarth[3]);

    glm::mat4 matMoon = m_moon->GetModelMatrix();
    glm::vec3 posMoon = glm::vec3(matMoon[3]);

    glm::vec3 dir = posMoon - posEarth;
    float dist = glm::length(dir);
    if (dist < 1e-6f) {
        glm::vec3 eye = posEarth + m_offset;
        m_camera->SetEye(eye.x, eye.y, eye.z);
    } else {
        dir = glm::normalize(dir);

        float sx = glm::length(glm::vec3(matEarth[0]));
        float sy = glm::length(glm::vec3(matEarth[1]));
        float sz = glm::length(glm::vec3(matEarth[2]));
        float radius = std::max(std::max(sx, sy), sz);

        float above = radius * 1.05f + 0.3f; 
        glm::vec3 eye = posEarth + dir * above;
        m_camera->SetEye(eye.x, eye.y, eye.z);
    }

    m_camera->SetCenter(posMoon.x, posMoon.y, posMoon.z);

}
