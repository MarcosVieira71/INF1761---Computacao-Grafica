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
#include "Base.h"
#include "Orbit.h"
#include "MoonGlobe.h"
#include "AstralBody.h"
#include "Texture.h"
#include "AstralEngine.h"
#include "Emissive.h"

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

	GLFWwindow* window = glfwCreateWindow(1600, 900, "T5", nullptr, nullptr);
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
	glViewport(0, 0, 1600, 900);

	auto light = Light::Make(0.0f, 0.0f, 0.0f, 1.0f, "world");
	light->SetAmbient(0.3f, 0.3f, 0.3f);
	light->SetDiffuse(1.0f, 1.0f, 0.9f);
	light->SetSpecular(1.0f, 1.0f, 0.9f);

	auto shader = Shader::Make(light, "world");
	shader->AttachVertexShader("../shaders/vertex.glsl");
	shader->AttachFragmentShader("../shaders/fragment.glsl");
	shader->Link();


    TablePtr table = Table::Make({0.0f, 0.0f, 0.0f}, Texture::Make("decal", "../textures/oak.jpg"), Texture::Make("decal", "../textures/metal.jpg"));
	MoonGlobePtr globe = MoonGlobe::Make({-2.0f, 1.0f, 2.0f}, Texture::Make("decal", "../textures/red.jpg"), {Texture::Make("decal", "../textures/moon.jpg")});

	BasePtr  base = Base::Make(1,1,{0.0f, 0.6f, 0.0f}, {0.10f,1.0f, 0.10f}, Texture::Make("decal", "../textures/base.jpg"));
	base->setup(globe);
	
	OrbitPtr orbSun = Orbit::Make();
	AstralBodyPtr astroSun = AstralBody::Make({0.0f, 1.0f, 0.0f}, {0.75f,0.75f, 0.75f}, {Texture::Make("decal", "../textures/sun.jpg"), 
		Emissive::Make(1.0f, 1.0f, 1.0f)});

   	base->setup(orbSun);

	OrbitPtr orbEarth = Orbit::Make();
	AstralBodyPtr astroEarth = AstralBody::Make({4.0f, 0.0f, 0.0f}, {0.5f, -0.5f, 0.5f}, {Texture::Make("decal", "../textures/earth.jpg"), Emissive::Make(0.0f, 0.0f, 0.0f)});


	orbSun->setup(astroSun);
	astroSun->setup(orbEarth);
	orbEarth->setup(astroEarth);
	table->setup(base);

	light->SetReference(astroSun);



	auto root = Node::Builder()
					.WithShader(shader)
					.AddNode(table)
					.Build();

	auto scene = Scene::Make(root);

	AstralEnginePtr engine = AstralEngine::Make();

	engine->addAxis(astroEarth, 30);
	engine->addOrbit(orbEarth, 30);

	scene->AddEngine(engine);

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

