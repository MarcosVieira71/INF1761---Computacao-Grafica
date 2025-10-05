#include "PoolEngine.h"
#include "Ball.h"
#include "Node.h"
#include "Transform.h"

EnginePtr PoolEngine::Make(std::map<std::shared_ptr<Ball>, TransformPtr> balls, glm::vec3 rightWall, glm::vec3 leftWall, glm::vec3 floor)
{
    return EnginePtr(new PoolEngine(balls, rightWall, leftWall, floor));
}


void PoolEngine::Update(float dt)
{
    for(auto [ball, transform] : _ballsMap){
        applyForces(*ball);
    }
    
    for(auto [ball, transform] : _ballsMap){
        verletIntegrate(*ball, dt);
    }

    checkBallCollisions();

    for(auto [ball, transform] : _ballsMap){
        checkWallCollision(*ball);
    }

    for(auto [ball, transform] : _ballsMap){
        const glm::vec3 pos = ball->position;
        transform->LoadIdentity(); 
        transform->Translate(pos.x, pos.y, pos.z); // apenas atualiza posição
    }
}

void PoolEngine::verletIntegrate(Ball& ball, float dt)
{
    glm::vec3 temp = ball.position;
    ball.position = ball.position + (ball.position - ball.last_position) + ball.acceleration * dt * dt;
    ball.last_position = temp;
    ball.acceleration = glm::vec3(0.0f);
}

void PoolEngine::applyForces(Ball& ball)
{
    glm::vec3 gravity(0.0f, -9.8f, 0.0f);
    ball.acceleration += gravity;
}


void PoolEngine::checkBallCollisions()
{
    for (auto it1 = _ballsMap.begin(); it1 != _ballsMap.end(); it1++)
    {
        auto ball1 = it1->first;
        for (auto it2 = std::next(it1); it2 != _ballsMap.end(); it2++)
        {
            auto ball2 = it2->first;

            glm::vec3 delta = ball2->position - ball1->position;
            float dist = glm::length(delta);
            float penetration = ball1->radius + ball2->radius - dist;

            if (penetration > 0.0f)
            {
                glm::vec3 norm = delta / dist;

                ball1->position -= 0.5f * penetration * norm;
                ball2->position += 0.5f * penetration * norm;
            }
        }
    }
}


void PoolEngine::checkWallCollision(Ball& ball)
{
    float radius = ball.radius;

    if (ball.position.x - radius < m_leftWall.x)
    {
        ball.position.x = m_leftWall.x + radius;
    }

    if (ball.position.x + radius > m_rightWall.x)
    {
        ball.position.x = m_rightWall.x - radius;
    }

    if (ball.position.y - radius < m_floor.y)
    {
        ball.position.y = m_floor.y + radius;
    }

}

PoolEngine::PoolEngine(std::map<std::shared_ptr<Ball>, TransformPtr> balls, glm::vec3 rightWall, glm::vec3 leftWall, glm::vec3 floor)
    : _ballsMap(balls), m_rightWall(rightWall), m_leftWall(leftWall), m_floor(floor)
{
}

PoolEngine::~PoolEngine()
{

}