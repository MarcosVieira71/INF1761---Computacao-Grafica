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
    std::map<Ball*, TransformPtr> _ballsMap;
    glm::vec3 m_rightWall;
    glm::vec3 m_leftWall;
    glm::vec3 m_floor;

    void verletIntegrate(Ball& ball, float dt);
    void applyForces(Ball& ball);
    void checkWallCollision(Ball& ball);
    void checkBallCollisions();

    PoolEngine(std::map<Ball*, TransformPtr> balls, glm::vec3 rightWall, glm::vec3 leftWall, glm::vec3 floor);

public:
    void Update(float dt) override;
    static EnginePtr Make(std::map<Ball*, TransformPtr> balls, glm::vec3 rightWall, glm::vec3 leftWall, glm::vec3 floor);
    ~PoolEngine();
};

