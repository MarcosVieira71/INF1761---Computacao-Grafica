#include "Camera3D.h"
#include "Arcball.h"
#include "Scene.h"
#include "Node.h"
#include "Shader.h"
#include "Color.h"
#include "Transform.h"
#include "Cube.h"
#include "Sphere.h"
#include "Light.h"
#include "Cylinder.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



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

	auto trfCube = Transform::Make();
	trfCube->Translate(-1.0f, 0.0f, 0.0f);
	trfCube->Scale(1.0f, 1.0f, 1.0f);

	auto cube = Node::Builder()
					.WithTransform(trfCube)
					.AddAppearance(Color::Make(1.0f, 0.3f, 0.3f))
					.AddShape(Cube::Make())
					.Build();
	auto trfSphere = Transform::Make();
	trfSphere->Translate(1.5f, 0.5f, 0.0f);
	trfSphere->Scale(0.8f, 0.8f, 0.8f);

	auto sphere = Node::Builder()
					  .WithTransform(trfSphere)
					  .AddAppearance(Color::Make(0.3f, 1.0f, 0.3f))
					  .AddShape(Cylinder::Make(1.0f, 2.0f, 32, true))
					  .Build();

	auto root = Node::Builder()
					.WithShader(shader)
					.AddNode(cube)
					.AddNode(sphere)
					.Build();

	auto scene = Scene::Make(root);

	auto camera = Camera3D::Make(0.0f, 0.0f, 6.0f);
	camera->SetCenter(0.0f, 0.0f, 0.0f);
	camera->SetAngle(45.0f);

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

