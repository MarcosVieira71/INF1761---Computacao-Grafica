#include <memory>

class Appearance;
class Camera2D;
class Engine;
class Light;
class Node;
class Scene;
class Shape;
class Shader;
class State;
class Transform;

using AppearancePtr = std::shared_ptr<Appearance>;
using Camera2DPtr   = std::shared_ptr<Camera2D>;
using EnginePtr     = std::shared_ptr<Engine>;
using LightPtr      = std::shared_ptr<Light>;
using NodePtr       = std::shared_ptr<Node>;
using ScenePtr      = std::shared_ptr<Scene>;
using ShapePtr      = std::shared_ptr<Shape>;
using ShaderPtr     = std::shared_ptr<Shader>;
using StatePtr      = std::shared_ptr<State>;
using TransformPtr  = std::shared_ptr<Transform>;

class SolarSystem
{
private:
    Camera2DPtr m_camera;
    EnginePtr m_solarEngine;
    ScenePtr m_scene;

    TransformPtr m_trfEarthOrbit;
    TransformPtr m_trfEarthGeometry;
    TransformPtr m_trfSun;
    TransformPtr m_trfMoonOrbit;
    TransformPtr m_trfMoonGeometry;

    LightPtr m_sunLight;
    ShaderPtr m_shader;

    NodePtr m_sun;
    NodePtr m_earth;
    NodePtr m_earthOrbit;
    NodePtr m_moon;
    NodePtr m_moonOrbit;
    NodePtr m_root;


public:
    SolarSystem();
    void engineSetup();
    void moonSetup();
    void earthSetup();
    void lightSetup();
    void sceneSetup();
    void shaderSetup();
    void sunSetup();
    void run(float dt);
};