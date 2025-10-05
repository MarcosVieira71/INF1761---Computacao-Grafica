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

class PoolSimulator
{
private:
    ScenePtr  m_scene;
    CameraPtr m_camera;
    ShaderPtr m_shader;
    EnginePtr m_engine;

    NodePtr m_container;

    NodePtr m_root;


    void setupContainer();
    void setupShader();

    void setupScene();
    
    
public:    
    void run(float dt);
    PoolSimulator();
    ~PoolSimulator();
};

