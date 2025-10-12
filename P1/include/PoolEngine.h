#include "Engine.h"
#include <memory>
#include <map>

#include <glm/glm.hpp>

class Transform;
using TransformPtr = std::shared_ptr<Transform>;

class Ball;

class PoolEngine : public Engine
{
private:
    std::map<std::shared_ptr<Ball>, TransformPtr> _ballsMap;
    glm::vec2 m_rightWall;
    glm::vec2 m_leftWall;
    glm::vec2 m_floor;

    void verletIntegrate(Ball& ball, float dt);
    void applyForces(Ball& ball);
    void checkWallCollision(Ball& ball);
    void checkBallCollisions();

    PoolEngine(std::map<std::shared_ptr<Ball>, TransformPtr> balls, glm::vec2 rightWall, glm::vec2 leftWall, glm::vec2 floor);

public:
    void Update(float dt) override;
    static EnginePtr Make(std::map<std::shared_ptr<Ball>, TransformPtr> balls, glm::vec2 rightWall, glm::vec2 leftWall, glm::vec2 floor);
    ~PoolEngine();
};

