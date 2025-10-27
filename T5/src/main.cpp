#include "Camera3D.h"
#include "Arcball.h"
#include "Scene.h"
#include "Node.h"
#include "Shader.h"
#include "Color.h"
#include "Transform.h"
#include "Table.h"
#include "Light.h"
#include "Cylinder.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static Camera3DPtr g_camera = nullptr;
static ArcballPtr g_arcball = nullptr;

static void cursorpos(GLFWwindow* win, double x, double y);
static void cursorinit(GLFWwindow* win, double x, double y);

static void mousebutton(GLFWwindow* win, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetCursorPosCallback(win, cursorinit);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		glfwSetCursorPosCallback(win, nullptr);
	}
}

static void cursorinit(GLFWwindow* win, double x, double y)
{
	int wn_w, wn_h, fb_w, fb_h;
	glfwGetWindowSize(win, &wn_w, &wn_h);
	glfwGetFramebufferSize(win, &fb_w, &fb_h);
	double xf = x * double(fb_w) / double(wn_w);
	double yf = (wn_h - y) * double(fb_h) / double(wn_h);
	if (g_arcball) g_arcball->InitMouseMotion(int(xf), int(yf));
	glfwSetCursorPosCallback(win, cursorpos);
}

static void cursorpos(GLFWwindow* win, double x, double y)
{
	int wn_w, wn_h, fb_w, fb_h;
	glfwGetWindowSize(win, &wn_w, &wn_h);
	glfwGetFramebufferSize(win, &fb_w, &fb_h);
	double xf = x * double(fb_w) / double(wn_w);
	double yf = (wn_h - y) * double(fb_h) / double(wn_h);
	if (g_arcball) g_arcball->AccumulateMouseMotion(int(xf), int(yf));
}



int main()
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "T5 - Simple 3D Scene", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW\n";
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 800, 600);

	auto light = Light::Make(5.0f, 5.0f, 5.0f, 1.0f, "world");
	light->SetAmbient(0.2f, 0.2f, 0.2f);
	light->SetDiffuse(0.9f, 0.9f, 0.9f);

	auto shader = Shader::Make(light, "world");
	shader->AttachVertexShader("../shaders/vertex.glsl");
	shader->AttachFragmentShader("../shaders/fragment.glsl");
	shader->Link();
    TablePtr table = Table::Make({0.0f, 0.f, 0.f});
    table->setupLegs();
	auto root = Node::Builder()
					.WithShader(shader)
					.AddNode(table)
					.Build();

	auto scene = Scene::Make(root);

	auto camera = Camera3D::Make(0.0f, 0.0f, 6.0f);
	camera->SetCenter(0.0f, 0.0f, 0.0f);
	camera->SetAngle(45.0f);

	g_camera = camera;
	g_arcball = camera->CreateArcball();
	glfwSetMouseButtonCallback(window, mousebutton);

	float t0 = float(glfwGetTime());
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float t = float(glfwGetTime());
		scene->Update(t - t0);
		t0 = t;
		scene->Render(camera);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

