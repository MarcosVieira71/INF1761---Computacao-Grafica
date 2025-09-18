#pragma once

#include "Circle.h"
#include "shader.h"
#include "Hand.h"

#include <glm/glm.hpp>
#include <memory>

class Clock;
using ClockPtr = std::shared_ptr<Clock>;

class Clock {
public:
    Clock(const glm::vec2& center, float radius);
    ~Clock() = default;

    static ClockPtr Make(const glm::vec2& center, float radius);

    void Update(); 
    void Draw(const ShaderPtr& shd, const glm::mat4& proj) const;

private:
    glm::vec2 m_center;
    float m_radius;

    CirclePtr m_face;
    CirclePtr m_dot;
    HandPtr m_hourHand;
    HandPtr m_minuteHand;
    HandPtr m_secondHand;

    float m_hourAngle   = 0.0f;
    float m_minuteAngle = 0.0f;
    float m_secondAngle = 0.0f;
};
