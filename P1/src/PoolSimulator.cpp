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
#include "PoolEngine.h"

#include <map>
#include <vector>


PoolSimulator::PoolSimulator()
{
    setupShader();
    setupContainer();
    setupBalls();
    setupScene();

}


PoolSimulator::~PoolSimulator()
{
}

void PoolSimulator::setupShader()
{
    m_shader = Shader::Make();
    m_shader->AttachVertexShader("../shaders/vertex.glsl");
    m_shader->AttachFragmentShader("../shaders/fragment.glsl");
    m_shader->Link();
}


void PoolSimulator::setupContainer()
{
    
    TransformPtr leftTransform = Transform::Make();
    leftTransform->Translate(-width/2, 0.0f, 0.0f);
    leftTransform->Scale(0.03, 3.3, 1.0f);

    TransformPtr rightTransform = Transform::Make();
    rightTransform->Translate(width/2, 0.0f, 0.0f);
    rightTransform->Scale(0.03, 3.3, 1.0f);

    TransformPtr floorTransform = Transform::Make();
    floorTransform->Translate(0.0f, -8.0f, 0.0f); 
    floorTransform->Scale(1.0f, 0.1f, 1.0f);  

    NodePtr leftNode = Node::Builder()
                    .WithShader(m_shader)
                    .AddShape(Rectangle::Make(width, height))
                    .WithTransform(leftTransform)
                    .AddAppearance(Color::Make(1,1,1))
                    .Build();

    NodePtr rightNode = Node::Builder()
                        .WithShader(m_shader)
                        .AddShape(Rectangle::Make(width, height))
                        .WithTransform(rightTransform)
                        .AddAppearance(Color::Make(1,1,1))
                        .Build();


    NodePtr floor = Node::Builder()
                    .WithShader(m_shader)
                    .AddShape(Rectangle::Make(width, height))
                    .AddAppearance(Color::Make(1,1,1))
                    .WithTransform(floorTransform)
                    .Build();
    
    m_container = Node::Builder()
                    .AddNode(floor)
                    .AddNode(rightNode)
                    .AddNode(leftNode)
                    .Build();
}

void PoolSimulator::setupBalls()
{
    std::map<std::shared_ptr<Ball>, TransformPtr> ballNodeMap;

    int numBalls = 150;
    float radius = 0.5f;

    for (int i = 0; i < numBalls; ++i)
    {
        float x = -5.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f;
        float y = 0.5f + i * (radius * 2.1f);
        float z = 0.0f;
        glm::vec3 pos(x, y, z);

        std::shared_ptr<Ball> ball = std::make_shared<Ball>(pos, radius);

        auto ballTransform = Transform::Make();
        ballTransform->Translate(x, y, z);

        NodePtr node = Node::Builder()
            .WithShader(m_shader)
            .AddShape(Disk::Make(radius, 64))
            .AddAppearance(Color::Make(1,0,1))
            .WithTransform(ballTransform)
            .Build();

        m_container->AddNode(node);

        ballNodeMap[ball] = ballTransform;
    }

    glm::vec3 leftWallPos(-width/2 + 0.3, 0.0f, 0.0f);
    glm::vec3 rightWallPos(width/2 - 0.3, 0.0f, 0.0f);
    glm::vec3 floorPos(0.0f, -8.0f + 0.25, 0.0f);
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
