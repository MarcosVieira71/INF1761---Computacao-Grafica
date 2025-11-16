#include "SolarSystemScene.h"

#include "Camera3D.h"
#include "Arcball.h"
#include "Node.h"
#include "SkyBox.h"
#include "Shader.h"
#include "Image.h"
#include "Texcube.h"
#include "Color.h"
#include "Transform.h"
#include "Light.h"
#include "Cylinder.h"
#include "Orbit.h"
#include "AstralBody.h"
#include "Sphere.h"
#include "Cube.h"
#include "Texture.h"
#include "AstralEngine.h"
#include "Emissive.h"
#include "CameraEngine.h"
#include "AsteroidBelt.h"
#include "SolarShaders.h"
#include "PlanetBuilder.h"

#include <array>
#include <random>
#include <iostream>

SolarSystemScene::SolarSystemScene()
{
    auto light = SolarShaders::CreateLight();
    auto shaders = SolarShaders::CreateShaders(light);

    ShapePtr sphere = PlanetBuilder::CreateSphere();
    OrbitPtr orbSun = Orbit::Make();
    AstralBodyPtr astroSun = AstralBody::Make(
        {0.0f, 1.0f, 0.0f},
        {128.0, 128.0, 128.0},
        {Texture::Make("decal", "../textures/sun.jpg"),
         Emissive::Make(1.0f, 1.0f, 1.0f)},
        sphere);
    orbSun->setup(astroSun);
    _ptr_map["sun"] = {orbSun, astroSun};

    PlanetBuilder::BuildPlanets(_ptr_map, sphere, orbSun, astroSun);

    AstralEnginePtr engine = CreateEngines();

    auto nonEmissive = Emissive::Make(0.f, 0.f, 0.f);
    AsteroidBelt belt(engine, astroSun, _ptr_map, shaders.normal, nonEmissive);
    belt.Generate();

    PlanetBuilder::BuildMoonAndRings(_ptr_map, sphere, shaders.normal);

    FinalizeScene(shaders.main, shaders.sky, orbSun, engine, sphere);
}


AstralEnginePtr SolarSystemScene::CreateEngines()
{
    AstralEnginePtr engine = AstralEngine::Make();
    std::array<float, 8> speedsOrbit = {47.9f, 35.0f, 29.8f, 24.1f, 13.1f, 9.7f, 6.8f, 5.4f};
    std::array<float, 8> speedsAxis = {10.f, 6.5f, 60.f, 40.f, 100.f, 80.f, 70.f, 65.f};

    std::array<std::string, 8> names = {"mercury", "venus", "earth",
                                        "mars", "jupiter", "saturn", "uranus", "neptune"};

    for (size_t j = 0; j < names.size(); ++j)
    {
        const auto &name = names[j];
        const auto &[orbit, astro] = _ptr_map[name];
        // std::cout << "Nome: " << name << " Órbita: " << speedsOrbit[j] << " Eixo: " << speedsAxis[j] << "\n";
        engine->addAxis(astro, speedsAxis[j]);
        engine->addOrbit(orbit, speedsOrbit[j]);
    }

    return engine;
}

void SolarSystemScene::FinalizeScene(const ShaderPtr &shader, const ShaderPtr &skyShader, OrbitPtr orbSun, AstralEnginePtr engine, const ShapePtr &sphere)
{
    engine->setEarth(std::get<1>(_ptr_map.at("earth")));
    engine->setMoon(std::get<1>(_ptr_map.at("moon")));
    engine->addAxis(std::get<1>(_ptr_map.at("sun")), 2.98f);

    auto skyApp = TexCube::Make("skybox", "../textures/space_cubemap.png");
    auto skyNode = Node::Builder().WithShader(skyShader).AddAppearance(skyApp).AddShape(SkyBox::Make()).Build();
    auto root = Node::Builder().WithShader(shader).AddNode(skyNode).AddNode(orbSun).Build();
    _scene = Scene::Make(root);
    _scene->AddEngine(engine);

    _cameraAlternative = Camera3D::Make(0.0f, 0.0f, 0.0f);
    _cameraAlternative->SetAngle(45.0f);
    _cameraAlternative->SetZPlanes(0.1f, 3000.0f);
    _cameraEngine = CameraEngine::Make(_cameraAlternative, std::get<1>(_ptr_map.at("earth")), std::get<1>(_ptr_map.at("moon")), glm::vec3(0.0f, 0.0f, 0.0f));
    _scene->AddEngine(_cameraEngine);

    _camera = Camera3D::Make(-1000.0f, 750.0f, 500.0f);
    _camera->SetCenter(0.0f, 0.0f, 0.0f);
    _camera->SetAngle(45.0f);
    _camera->SetZPlanes(1, 3000.0f);

    _activeCamera = _camera;
    _arcball = _camera->CreateArcball();
}

ScenePtr SolarSystemScene::GetScene() const 
{ 
    return _scene; 
}

Camera3DPtr SolarSystemScene::GetCamera() const 
{ 
    return _camera; 
}

Camera3DPtr SolarSystemScene::GetCameraAlternative() const 
{ 
    return _cameraAlternative; 
}
Camera3DPtr SolarSystemScene::GetActiveCamera() const 
{ 
    return _activeCamera; 
}
CameraEnginePtr SolarSystemScene::GetCameraEngine() const 
{ 
    return _cameraEngine; 
}
ArcballPtr SolarSystemScene::GetArcball() const 
{ 
    return _arcball; 
}
const std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>>& SolarSystemScene::GetPtrMap() const 
{ 
    return _ptr_map; 
}

void SolarSystemScene::SetActiveCamera(const Camera3DPtr &c) 
{ 
    _activeCamera = c; 
}
