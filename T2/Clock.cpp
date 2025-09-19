#include "Clock.h"

#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Clock::Clock(const glm::vec2& center, float radius, bool continuous)
    : m_center(center), m_radius(radius), m_continuous(continuous)
{
    m_face = Circle::Make(radius, 60);
    m_dot = Circle::Make(radius * 0.045f, 64);
    m_hourHand   = Hand::Make(radius * 0.4f, radius * 0.07f);
    m_minuteHand = Hand::Make(radius * 0.6f, radius * 0.07f);
    m_secondHand = Hand::Make(radius * 0.8f, radius * 0.07f);
}

ClockPtr Clock::Make(const glm::vec2& center, float radius, bool continuous) {
    return std::make_shared<Clock>(center, radius, continuous);
}

void Clock::Update()
{
    using namespace std::chrono;

    auto toAngle = [](float value, float max) {
        return glm::radians(360.0f * (value / max));
    };

    auto now = system_clock::now();
    time_t t = system_clock::to_time_t(now);
    tm localTime = *localtime(&t);

    if (m_continuous) {
        auto epoch = now.time_since_epoch();
        auto frac = duration_cast<milliseconds>(epoch).count() % 1000 / 1000.0f;


        float hours   = (localTime.tm_hour % 12) 
                        + localTime.tm_min / 60.0f 
                        + (localTime.tm_sec + frac) / 3600.0f;
        float minutes = localTime.tm_min 
                        + (localTime.tm_sec + frac) / 60.0f;
        float seconds = localTime.tm_sec + frac;

        m_hourAngle   = toAngle(hours, 12.0f);
        m_minuteAngle = toAngle(minutes, 60.0f);
        m_secondAngle = toAngle(seconds, 60.0f);
    } else {
        float hours   = static_cast<float>(localTime.tm_hour % 12) + localTime.tm_min / 60.0f;
        float minutes = static_cast<float>(localTime.tm_min) + localTime.tm_sec / 60.0f;
        float seconds = static_cast<float>(localTime.tm_sec);

        m_hourAngle   = toAngle(hours, 12.0f);
        m_minuteAngle = toAngle(minutes, 60.0f);
        m_secondAngle = toAngle(seconds, 60.0f);
    }
}

void Clock::Draw(const ShaderPtr& shd, const glm::mat4& proj) const
{
    glm::mat4 baseModel = glm::translate(glm::mat4(1.0f), glm::vec3(m_center, 0.0f));

    // face
    shd->SetUniform("proj", proj);
    shd->SetUniform("model", baseModel);
    shd->SetUniform("uniformColor", glm::vec3(1.0f));
    m_face->Draw();
    
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
    shd->SetUniform("model", baseModel);
    shd->SetUniform("uniformColor", glm::vec3(1.0f,0.0f,0.0f));
    m_dot->Draw();
}
