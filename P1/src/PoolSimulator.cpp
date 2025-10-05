#include "PoolSimulator.h"

#include "Node.h"
#include "Rectangle.h"
#include "Color.h"
#include "Transform.h"
#include "Shader.h"
#include "Scene.h"
#include "Camera2D.h"


PoolSimulator::PoolSimulator()
{
    setupShader();
    setupContainer();
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
    float width = 20.0f;      
    float height = 5.0f;      

    
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


void PoolSimulator::setupScene()
{
    m_root = Node::Builder()
            .WithShader(m_shader)
            .AddNode(m_container)
            .Build();
    
    m_scene = Scene::Make(m_root);
    // m_scene->AddEngine(PoolPhysicsEngine::Make());
    m_camera = Camera2D::Make(-10, 10, -10, 10);
}

void PoolSimulator::run(float dt)
{
    m_scene->Update(dt);
    m_scene->Render(m_camera);

}