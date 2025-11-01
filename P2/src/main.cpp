#include "Camera3D.h"
#include "Arcball.h"
#include "Scene.h"
#include "Node.h"
#include "Shader.h"
#include "Color.h"
#include "Transform.h"
#include "Light.h"
#include "Cylinder.h"
#include "Orbit.h"
#include "AstralBody.h"
#include "Sphere.h"
#include "Texture.h"
#include "AstralEngine.h"
#include "Emissive.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <array>
#include <string>
#include <map>
#include <utility>


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
	light->SetAmbient(0.1f, 0.1f, 0.1f);
	light->SetDiffuse(1.0f, 1.0f, 0.9f);
	light->SetSpecular(1.0f, 1.0f, 0.9f);

	auto shader = Shader::Make(light, "world");
	shader->AttachVertexShader("../shaders/vertex.glsl");
	shader->AttachFragmentShader("../shaders/fragment.glsl");
	shader->Link();

	std::array<std::string, 8> names = {"mercury", "venus", "earth", 
		"mars", "jupiter", "saturn", "uranus", "neptune"}; 
	

	ShapePtr sphere = Sphere::Make(64,64);

	OrbitPtr orbSun = Orbit::Make();
	AstralBodyPtr astroSun = AstralBody::Make(
		{0.0f, 1.0f, 0.0f}, 
		{64.0, 64.0, 64.0}, 
		{
			Texture::Make("decal", "../textures/sun.jpg"), 
			Emissive::Make(1.0f, 1.0f, 1.0f)
		},
		sphere
	);

	orbSun->setup(astroSun);

	std::array<glm::vec3,8> scales = {
		glm::vec3{0.003f, 0.003f, 0.003f} * 25.0f,
		glm::vec3{0.008f, 0.008f, 0.008f} * 25.0f,
		glm::vec3{0.009f, -0.009f, 0.009f} * 25.0f,
		glm::vec3{0.005f, 0.005f, 0.005f} * 25.0f,
		glm::vec3{0.10f, 0.10f, 0.10f} * 5.0f,
		glm::vec3{0.08f, 0.08f, 0.08f} * 5.0f,
		glm::vec3{0.04f, 0.04f, 0.04f}* 10.0f,
		glm::vec3{0.03f, 0.03f, 0.03f} * 10.0f

	};


	std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>> ptr_map;

	int i = 0;
	int k = 0;
	for(const auto& planet : names)
	{
		i++;
		std::string texturePath = "../textures/" + planet + ".jpg";
		
		const glm::vec3 pos = {1.f + i, 0.f, 0.f}; 

		const std::vector<AppearancePtr> apps = {
			Emissive::Make(0.f, 0.f, 0.f),
			Texture::Make("decal", texturePath)
		};

		const glm::vec3 scale = scales[k];
		auto astro = AstralBody::Make(std::move(pos), std::move(scale), std::move(apps), sphere);
		auto orbit = Orbit::Make();
		
		ptr_map[planet] = {orbit, astro};
		orbit->setup(astro);
		astroSun->setup(orbit);
		k++;
	}

	AstralEnginePtr engine = AstralEngine::Make();

	
	int j = 0;
	std::array<float,8> speedsOrbit = {47.9f, 35.0f, 29.8f, 24.1f, 13.1f, 9.7f, 6.8f, 5.4f};
	std::array<float,8> speedsAxis = {10.f, 6.5f, 60.f, 40.f, 100.f, 80.f, 70.f, 65.f};
	
	for (size_t j = 0; j < names.size(); ++j)
	{
		const auto& name = names[j];
		const auto& [orbit, astro] = ptr_map[name];


		std::cout << "Nome: " << name
				<< " Órbita: " << speedsOrbit[j]
				<< " Eixo: " << speedsAxis[j] << "\n";

		engine->addAxis(astro, speedsAxis[j]);
		engine->addOrbit(orbit, speedsOrbit[j]);
	}

	OrbitPtr orbMoon = Orbit::Make();
	AstralBodyPtr astroMoon = AstralBody::Make(
		{2.0f, 0.0f, 0.0f}, 
		{0.27f, 0.27f, 0.27f}, 
		{
			Texture::Make("decal", "../textures/moon.jpg"), 
			Emissive::Make(0.f, 0.f, 0.f)
		},
		sphere
	);
	orbMoon->setup(astroMoon);

	const auto& [orbit, astro] = ptr_map["earth"];
	astro->setup(orbMoon);
	engine->addAxis(astroMoon, 1.0f);
	engine->addOrbit(orbMoon, 1.0f);



		
	auto root = Node::Builder()
		.WithShader(shader)
		.AddNode(orbSun)
		.Build();
	
	auto scene = Scene::Make(root);
	
	scene->AddEngine(engine);


	auto camera = Camera3D::Make(-600.0f, 0.0f, 0.0f);
	camera->SetCenter(0.0f, 0.0f, 0.0f);
	camera->SetAngle(45.0f);
	camera->SetZPlanes(1, 2500.0f);

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

