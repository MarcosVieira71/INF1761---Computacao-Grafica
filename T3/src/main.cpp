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
#include "Transform.h"

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <memory>
#include <iostream>

int main() {

    // 1️⃣ Inicializa GLFW
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
    
    glewExperimental = GL_TRUE;  // ativa algumas extensões modernas
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);

    auto trf1 = Transform::Make();

    ShaderPtr shader = Shader::Make();
    shader->AttachVertexShader("../shaders/vertex.glsl");
    shader->AttachFragmentShader("../shaders/fragment.glsl");
    shader->Link();

    NodePtr sunNode = Node::Builder()
                        .WithTransform(trf1)
                        .AddAppearance(Color::Make(1,1,1))
                        .AddShape(Disk::Make(1.0f, 32))
                        .Build();

    NodePtr rootNode = Node::Builder()
                        .WithShader(shader)
                        .AddNode(sunNode)
                        .Build();

    ScenePtr scene = Scene::Make(rootNode);
    Camera2DPtr camera2D = Camera2D::Make(0,10,0,10);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->Render(camera2D);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
