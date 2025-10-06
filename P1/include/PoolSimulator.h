#include <memory>

class Node;
using NodePtr = std::shared_ptr<Node>;

class Scene;
using ScenePtr = std::shared_ptr<Scene>;

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

class Shader;
using ShaderPtr = std::shared_ptr<Shader>;

class Engine;
using EnginePtr = std::shared_ptr<Engine>;

class Light;
using LightPtr = std::shared_ptr<Light>;

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class PoolSimulator
{
private:
    TexturePtr m_ballTex;
    TexturePtr m_containerTex;

    LightPtr  m_light;
    ScenePtr  m_scene;
    CameraPtr m_camera;
    ShaderPtr m_shader;
    EnginePtr m_engine;

    NodePtr m_container;
    NodePtr m_balls;
    NodePtr m_root;
    

    //atributos relacionados a dimensoes do conteiner
    float m_width = 20.0f;      
    float m_height = 5.0f;    

    void setupLight();
    void setupContainer();
    void setupShader();
    void setupBalls();
    void setupScene();
    
    
public:    
    void run(float dt);
    PoolSimulator();
    ~PoolSimulator();
};

