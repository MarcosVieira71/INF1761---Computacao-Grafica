#include "SolarSystem.h"

#include "Camera2D.h"
#include "Color.h"
#include "Disk.h"
#include "Light.h"
#include "Node.h"
#include "SolarSystemEngine.h"
#include "Scene.h"
#include "Shader.h"
#include "Transform.h"

SolarSystem::SolarSystem()
{
    lightSetup();
    shaderSetup();
    earthSetup();
    moonSetup();
    sunSetup();
    engineSetup();
    sceneSetup();

}

void SolarSystem::lightSetup()
{
    m_sunLight = Light::Make(5.0f, 5.0f, 5.0f, 1.0f, "world");
    m_sunLight->SetAmbient(1.0f, 1.0f, 1.0f);
    m_sunLight->SetDiffuse(1.0f, 1.0f, 1.0f);
    m_sunLight->SetSpecular(1.0f, 1.0f, 1.0f);
}

void SolarSystem::shaderSetup()
{
    m_shader = Shader::Make(m_sunLight,"world");
    m_shader->AttachVertexShader("../shaders/vertex.glsl");
    m_shader->AttachFragmentShader("../shaders/fragment.glsl");
    m_shader->Link();
}

void SolarSystem::earthSetup()
{
    m_trfEarthOrbit    = Transform::Make();   
    m_trfEarthGeometry = Transform::Make();  
     
    m_trfEarthGeometry->Translate(4.0f, 0.0f, 0.0f);
    m_trfEarthGeometry->Scale(0.5f, 0.5f, 1.0f);

    m_earthOrbit = Node::Builder()
        .WithTransform(m_trfEarthOrbit)
        .Build();
;

    m_earth = Node::Builder()
        .WithTransform(m_trfEarthGeometry)
        .AddAppearance(Color::Make(0,0,1))
        .AddShape(Disk::Make(1.0f, 32))
        .Build();

    m_earthOrbit->AddNode(m_earth);
}

void SolarSystem::moonSetup()
{
    m_trfMoonOrbit = Transform::Make();
    m_trfMoonGeometry = Transform::Make();

    m_trfMoonGeometry->Translate(3.0f, 0.0f, 0.0f);
    m_trfMoonGeometry->Scale(0.3f, 0.3f, 1.0f);

    m_moonOrbit = Node::Builder()
        .WithTransform(m_trfMoonOrbit)
        .Build();

    m_moon = Node::Builder()
        .WithTransform(m_trfMoonGeometry)
        .AddAppearance(Color::Make(1,1,1))
        .AddShape(Disk::Make(1.0f, 32))
        .Build();

    m_earth->AddNode(m_moonOrbit);
    m_moonOrbit->AddNode(m_moon);
}

void SolarSystem::sunSetup()
{
    m_trfSun = Transform::Make();
    m_trfSun->Translate(5.0f,5.0f, 0);
    m_trfSun->Scale(0.8f,0.8f, 1);
    
    m_sun = Node::Builder()
            .WithTransform(m_trfSun)
            .AddAppearance(Color::Make(1.0f, 0.9f, 0.0f))
            .AddShape(Disk::Make(1.0f,32))
            .Build();
    m_sun->AddNode(m_earthOrbit);
}

void SolarSystem::engineSetup()
{
    m_solarEngine = SolarSystemEngine::Make(m_trfEarthOrbit, m_trfMoonOrbit);
}

void SolarSystem::sceneSetup()
{
    m_root = Node::Builder()
            .WithShader(m_shader)
            .AddNode(m_sun)
            .Build();
    
    m_scene = Scene::Make(m_root);
    m_scene->AddEngine(m_solarEngine);
    m_camera = Camera2D::Make(0,10,0,10);
}

void SolarSystem::run(float dt)
{
    m_scene->Update(dt);
    m_scene->Render(m_camera);

}