#include "PoolSimulator.h"

#include "Node.h"
#include "Rectangle.h"
#include "Color.h"
#include "Disk.h"
#include "Ball.h"
#include "Transform.h"
#include "Shader.h"
#include "Scene.h"
#include "Camera2D.h"
#include "Light.h"
#include "Texture.h"
#include "PoolEngine.h"

#include <map>
#include <vector>


PoolSimulator::PoolSimulator() : 
    m_ballTexGold(Texture::Make("decal", "../textures/golden_ball.png")),
    m_ballTexBricks(Texture::Make("decal", "../textures/bricks.jpg")),
    m_containerTex(Texture::Make("decal", "../textures/silver.jpg"))

{
    setupShader();
    setupBalls();
    setupContainer();
    setupScene();
}

PoolSimulator::~PoolSimulator()
{
}

void PoolSimulator::setupLight()
{
    m_light = Light::Make(0.0f, 0.0f, 5.0f, 1.0f);
    m_light->SetAmbient(1.0f, 1.0f, 1.0f);
    m_light->SetDiffuse(1.0f, 1.0f, 1.0f);
    m_light->SetSpecular(1.0f, 1.0f, 1.0f);
}

void PoolSimulator::setupShader()
{
    setupLight();
    m_shader = Shader::Make(m_light, "world");
    m_shader->AttachVertexShader("../shaders/vertex.glsl");
    m_shader->AttachFragmentShader("../shaders/fragment.glsl");
    m_shader->Link();
}

void PoolSimulator::setupContainer()
{
    
    TransformPtr leftTransform = Transform::Make();
    leftTransform->Translate(-m_width/2, 0.0f, 0.0f);
    leftTransform->Scale(0.03, 3.3, 1.0f);

    TransformPtr rightTransform = Transform::Make();
    rightTransform->Translate(m_width/2, 0.0f, 0.0f);
    rightTransform->Scale(0.03, 3.3, 1.0f);

    TransformPtr floorTransform = Transform::Make();
    floorTransform->Translate(0.0f, -8.0f, 0.0f);
    floorTransform->Scale(1.0f, 0.1f, 1.0f);

    NodePtr leftNode = Node::Builder()
                           .AddShape(Rectangle::Make(m_width, m_height))
                           .WithTransform(leftTransform)
                           .Build();

    NodePtr rightNode = Node::Builder()
                            .AddShape(Rectangle::Make(m_width, m_height))
                            .WithTransform(rightTransform)
                            .Build();

    NodePtr floor = Node::Builder()
                        .AddShape(Rectangle::Make(m_width, m_height))
                        .WithTransform(floorTransform)
                        .Build();

    m_container = Node::Builder()
                      .AddAppearance(Color::Make(1, 1, 1))
                      .AddAppearance(m_containerTex)
                      .AddNode(floor)
                      .AddNode(rightNode)
                      .AddNode(leftNode)
                      .AddNode(m_balls)
                      .Build();
}

void PoolSimulator::setupBalls()
{
    std::map<std::shared_ptr<Ball>, TransformPtr> ballNodeMap;

    int numBalls = 200;
    float radius = 0.5f;
    float minX = -m_width/2 + 0.3f + radius + 1.0f; // margem mínima da parede esquerda
    float maxX = m_width/2 - 0.3f - radius - 1.0f;  // margem máxima da parede direita
    m_balls = Node::Builder()
                    .AddAppearance(Color::Make(1,1,1))
                    .Build();

    for (int i = 0; i < numBalls; ++i)
    {
        float x = minX + static_cast<float>(rand()) / RAND_MAX * (maxX - minX);
        float y = radius + i * (radius * 2.1f); // garante que não comece no chão
        
        glm::vec2 pos(x, y);

        std::shared_ptr<Ball> ball = std::make_shared<Ball>(pos, radius);

        auto ballTransform = Transform::Make();
        ballTransform->Translate(x, y, 0);
        auto app = i % 2 == 0 ? m_ballTexGold : m_ballTexBricks;
        
        NodePtr balls = Node::Builder()
            .AddShape(Disk::Make(radius, 64))
            .AddAppearance(app)
            .WithTransform(ballTransform)
            .Build();

        m_balls->AddNode(balls);

        ballNodeMap[ball] = ballTransform;
    }

    glm::vec2 leftWallPos(-m_width/2 + 0.3, 0.0f);
    glm::vec2 rightWallPos(m_width/2 - 0.3, 0.0f);
    glm::vec2 floorPos(0.0f, -8.0f + 0.25);
    m_engine = PoolEngine::Make(ballNodeMap, rightWallPos, leftWallPos, floorPos);
}

void PoolSimulator::setupScene()
{
    m_root = Node::Builder()
                 .WithShader(m_shader)
                 .AddNode(m_container)
                 .Build();

    m_scene = Scene::Make(m_root);
    m_scene->AddEngine(m_engine);
    m_camera = Camera2D::Make(-10, 10, -10, 10);
}

void PoolSimulator::run(float dt)
{
    m_scene->Update(dt);
    m_scene->Render(m_camera);

}
