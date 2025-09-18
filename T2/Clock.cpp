#include "Clock.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Clock::Clock(const glm::vec2& center, float radius)
    : m_center(center), m_radius(radius)
{
    m_face = Circle::Make(center, radius, 60);
    m_dot = Circle::Make(center, radius * 0.03f, 30);
    m_hourHand   = Hand::Make(center, radius * 0.5f, radius * 0.05f);
    m_minuteHand = Hand::Make(center, radius * 0.7f, radius * 0.06f);
    m_secondHand = Hand::Make(center, radius * 0.85f, radius * 0.05f);
}

ClockPtr Clock::Make(const glm::vec2& center, float radius) {
    return std::make_shared<Clock>(center, radius);
}

void Clock::Update()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    time_t t = system_clock::to_time_t(now);
    tm localTime = *localtime(&t);

    float hours   = static_cast<float>(localTime.tm_hour % 12) + localTime.tm_min / 60.0f;
    float minutes = static_cast<float>(localTime.tm_min) + localTime.tm_sec / 60.0f;
    float seconds = static_cast<float>(localTime.tm_sec); 
    m_hourAngle   = glm::radians(360.0f * (hours / 12.0f));
    m_minuteAngle = glm::radians(360.0f * (minutes / 60.0f));
    m_secondAngle = glm::radians(360.0f * (seconds / 60.0f)); 
}

void Clock::Draw(const ShaderPtr& shd, const glm::mat4& proj) const
{
    glm::mat4 idModel = glm::mat4(1.0f);

    // face
    shd->SetUniform("proj", proj);
    shd->SetUniform("model", idModel);
    shd->SetUniform("uniformColor", glm::vec3(1.0f));
    m_face->Draw();

    glm::mat4 baseModel = glm::translate(glm::mat4(1.0f), glm::vec3(m_center, 0.0f));

    glm::mat4 hourModel   = glm::rotate(baseModel, -m_hourAngle,   glm::vec3(0,0,1));
    glm::mat4 minuteModel = glm::rotate(baseModel, -m_minuteAngle, glm::vec3(0,0,1));
    glm::mat4 secondModel = glm::rotate(baseModel, -m_secondAngle, glm::vec3(0,0,1));


    //horas
    shd->SetUniform("model", hourModel);
    shd->SetUniform("uniformColor", glm::vec3(0.0f));
    m_hourHand->Draw();

    // minutos
    shd->SetUniform("model", minuteModel);
    m_minuteHand->Draw();

    // segundos
    shd->SetUniform("model", secondModel);
    m_secondHand->Draw();

    // ponto central por cima de tudo
    shd->SetUniform("model", idModel);
    shd->SetUniform("uniformColor", glm::vec3(1.0f,0.0f,0.0f));
    m_dot->Draw();
}
