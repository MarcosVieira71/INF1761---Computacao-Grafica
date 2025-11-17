#include "SolarSystemScene.h"

#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif


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
#include "Framebuffer.h"
#include "TexDepth.h"

#include <array>
#include <random>
#include <iostream>

SolarSystemScene::SolarSystemScene()
{
    auto light = SolarShaders::CreateLight();
    _shaders = SolarShaders::CreateShaders(light);
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
    AsteroidBelt belt(engine, astroSun, _ptr_map, _shaders.normal, nonEmissive);
    belt.Generate();

    PlanetBuilder::BuildMoonAndRings(_ptr_map, sphere, _shaders.normal);

    FinalizeScene(_shaders.main, _shaders.sky, orbSun, engine, sphere);
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

	_shadowCamera = Camera3D::Make(0.0f, 1.0f, 0.0f);
	_shadowCamera->SetUpDir(1.0f, 0.0f, 0.0f);
	_shadowCamera->SetAngle(90.0f);
    _shadowCamera->SetZPlanes(0.1f, 500.0f);
    initShadowResources();

    
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

void SolarSystemScene::Render(float dt)
{
    glViewport(0, 0, 1600, 900);

    _scene->GetRoot()->SetShader(_shaders.main);
    _scene->Update(dt);
    _scene->Render(_activeCamera);
}

void SolarSystemScene::RenderShadow()
{
    _fbo->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 1024, 1024);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);

    _scene->GetRoot()->SetShader(_shaders.shadow);
    _scene->Render(_shadowCamera);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_CULL_FACE);
    glFlush();
    _fbo->Unbind();    
}

void SolarSystemScene::initShadowResources()
{
    _smap = TexDepth::Make("shadowMap", 4096, 4096);
    _smap->SetCompareMode();
    _fbo = Framebuffer::Make(_smap);

    // glm::mat4 bias = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f))
    //                * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    for (const auto& [name, pair] : _ptr_map)
    {
        auto& [orbit, astro] = pair;
        glm::mat4 modelMatrix = astro->GetTransform()->GetMatrix(); // posição, rotação, escala
        glm::mat4 lightSpace =  _shadowCamera->GetProjMatrix()
                                           * _shadowCamera->GetViewMatrix()
                                           * modelMatrix;
                                        //    * bias;

        auto mtex = Variable<glm::mat4>::Make("Mtex",lightSpace);
        astro->AddAppearance(mtex);
    }
}
