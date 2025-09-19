#ifdef _WIN32
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#endif

#include "error.h"
#include "shader.h"
#include "Clock.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <chrono>

static ClockPtr clock1;
static ClockPtr clock2;
static ShaderPtr shd;


static void error(int code, const char* msg)
{
    printf("GLFW error %d: %s\n", code, msg);
    glfwTerminate();
    exit(1);
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize(GLFWwindow* win, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void initialize()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glm::vec2 center(300.0f, 200.0f); 

    float radius = 150.0f;
    clock2 = Clock::Make(center, radius, true);

    shd = Shader::Make();
    shd->AttachVertexShader("shaders/vertex.glsl");
    shd->AttachFragmentShader("shaders/fragment.glsl");
    shd->Link();

    Error::Check("initialize");
}

static void display(GLFWwindow* win)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 proj = glm::ortho(0.0f, 600.0f, 0.0f, 400.0f, -1.0f, 1.0f);
    shd->UseProgram();

    clock2->Update();
    clock2->Draw(shd, proj);


    Error::Check("display");        
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required for macOS

    glfwSetErrorCallback(error);

    GLFWwindow* win = glfwCreateWindow(600, 400, "Clock", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(win, resize);
    glfwSetKeyCallback(win, keyboard);

    glfwMakeContextCurrent(win);

#ifdef __glad_h_
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD OpenGL context\n");
        exit(1);
    }
#endif
#ifdef __glew_h__
    glewInit();
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW OpenGL context\n");
        exit(1);
    }
#endif

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    initialize();

    while (!glfwWindowShouldClose(win)) {
        display(win);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
