#include "Appearance.h"
#include "Camera2D.h"
#include "Disk.h"
#include "Error.h"
#include "Node.h"
#include "Shader.h"
#include "Shape.h"
#include "Color.h"
#include "Scene.h"
#include "State.h"
#include "SolarSystemEngine.h"
#include "Transform.h"

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <memory>
#include <iostream>

int main() {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Mini Solar System", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    #ifdef _WIN32
    // Inicializa GLAD no Windows
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    #endif
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);

    auto trfSun = Transform::Make();
    trfSun->Translate(5.0f,5.0f, 1);
    trfSun->Scale(0.8f,0.8f, 1);
    
    ShaderPtr shader = Shader::Make();
    shader->AttachVertexShader("../shaders/vertex.glsl");
    shader->AttachFragmentShader("../shaders/fragment.glsl");
    shader->Link();

    NodePtr sunNode = Node::Builder()
                        .WithTransform(trfSun)
                        .AddAppearance(Color::Make(1,1,1))
                        .AddShape(Disk::Make(1.0f, 32))
                        .Build();

    auto trfEarth = Transform::Make();
    auto trfEarthOrbit = Transform::Make();

    auto trfMoon = Transform::Make();
    auto trfMoonOrbit = Transform::Make();

    trfEarth->Translate(4.0f, 0.0f, 0.0f);
    trfEarth->Scale(0.5f, 0.5f, 1.0f);

    trfMoon->Translate(3.0f, 0.0f, 0.0f);
    trfMoon->Scale(0.3f, 0.3f, 1.0f);

    NodePtr earthOrbitNode = Node::Builder()
        .WithTransform(trfEarthOrbit)
        .Build();

    NodePtr earthNode = Node::Builder()
        .WithTransform(trfEarth)
        .AddAppearance(Color::Make(0,0,1))
        .AddShape(Disk::Make(1.0f, 32))
        .Build();

    NodePtr moonOrbitNode = Node::Builder()
        .WithTransform(trfMoonOrbit)
        .Build();

    NodePtr moonNode = Node::Builder()
        .WithTransform(trfMoon)
        .AddAppearance(Color::Make(1,0,0))
        .AddShape(Disk::Make(1.0f, 32))
        .Build();


    sunNode->AddNode(earthOrbitNode);
    earthOrbitNode->AddNode(earthNode);

    earthNode->AddNode(moonOrbitNode);
    moonOrbitNode->AddNode(moonNode);

    auto engine = SolarSystemEngine::Make(trfSun, trfEarthOrbit, trfMoonOrbit);

    NodePtr rootNode = Node::Builder()
                        .WithShader(shader)
                        .AddNode(sunNode)
                        .Build();

    ScenePtr scene = Scene::Make(rootNode);
    scene->AddEngine(engine);
    Camera2DPtr camera2D = Camera2D::Make(0,10,0,10);
    
    float t0 = float(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float t = float(glfwGetTime());
        scene->Update(t-t0);
        t0 = t;
        scene->Render(camera2D);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
