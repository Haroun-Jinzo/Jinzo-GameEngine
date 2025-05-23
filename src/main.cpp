#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <stack>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Model.h"
#include "graphics/Light.h"

#include "graphics/models/Cube.hpp"
#include "graphics/models/Lamp.hpp"
#include "graphics/models/Sphere.hpp"
#include "graphics/models/Box.hpp"

#include "physics/Environment.h"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Camera.h"

#include "Algorithms/States.hpp"

#include "scene.h"

Scene scene;

void processInput(double dt);

Camera cam;

//Joystick mainJ(0);

double dt = 0.0f; // tme btwn frames
double lastFrame = 0.0f; // time of last frame

SphereArray launchObjects;

int main() {
	std::cout << "Hello, OpenGL!" << std::endl;

	scene = Scene(3, 3, "OpenGL Tutorial", 800, 600);
	if (!scene.init()) {
		std::cout << "Could not open window" << std::endl;
		glfwTerminate();
		return -1;
	}

	scene.cameras.push_back(&cam);
	scene.activeCamera = 0;

	// SHADERS===============================
	Shader shader("assets/object.vert", "assets/object.frag");
	Shader lampShader("assets/Instanced/Insttance.vert", "assets/Lamp.frag");
	Shader launchShader("assets/Instanced/Insttance.vert", "assets/object.frag");
	Shader boxShader("assets/Instanced/Box.vert", "assets/Instanced/Box.frag");

	// MODELS==============================
	launchObjects.init();

	Box box;
	box.init();

	Model m(BoundTypes::AABB, glm::vec3(0.0f), glm::vec3(0.05f));
	m.loadModel("assets/Models/helmet/scene.gltf");

	// LIGHTS
	DirLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) };
	scene.dirLight = &dirLight;

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec4 ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
	glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	glm::vec4 specular = glm::vec4(1.0f);
	float k0 = 1.0f;
	float k1 = 0.09f;
	float k2 = 0.032f;

	PointLight pointLights[4];

	LampArray lamps;
	lamps.init();
	for (unsigned int i = 0; i < 4; i++) {
		pointLights[i] = {
			pointLightPositions[i],
			k0, k1, k2,
			ambient, diffuse, specular
		};
		lamps.lightInstances.push_back(pointLights[i]);
		scene.pointLights.push_back(&pointLights[i]);
		States::Activate(&scene.activePointLights, i);
	}

	SpotLight spotLight = {
		cam.cameraPos, cam.cameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		1.0f, 0.07f, 0.032f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};
	scene.spotLights.push_back(&spotLight);
	scene.activeSpotLights = 1;	// 0b00000001

	while (!scene.ShouldClose()) {
		box.positions.clear();
		box.sizes.clear();

		// calculate dt
		double currentTime = glfwGetTime();
		dt = currentTime - lastFrame;
		lastFrame = currentTime;

		// process input
		processInput(dt);

		// update screen values
		scene.Update();

		scene.Render(shader, true);
		m.Render(shader, dt, &box);

		// launch objects
		std::stack<int> removeObjects;
		for (int i = 0; i < launchObjects.instances.size(); i++) {
			if (glm::length(scene.getActiveCamera()->cameraPos - launchObjects.instances[i].pos) > 50.0f) {
				removeObjects.push(i);
				continue;
			}
		}
		for (int i = 0; i < removeObjects.size(); i++) {
			launchObjects.instances.erase(launchObjects.instances.begin() + removeObjects.top());
			removeObjects.pop();
		}

		if (launchObjects.instances.size() > 0) {
			scene.Render(launchShader, true);
			launchObjects.Render(launchShader, dt, &box);
		}

		// lamps
		scene.Render(lampShader, false);
		lamps.Render(lampShader, dt, &box);

		// render boxes
		if (box.positions.size() > 0) {
			// instances exist
			scene.Render(boxShader, false);
			box.Render(boxShader);
		}

		std::cout << "box positions " << box.positions.size() << "  :  " << box.sizes.size() << std::endl;
		//rotate direction light 
		dirLight.direction.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		// send new frame to window
		scene.NewFrame();
	}

	// clean up objects
	lamps.CleanUp();
	box.cleanup();
	launchObjects.CleanUp();
	m.CleanUP();

	scene.Cleanup();
	return 0;
}

void launchItem(float dt) {
	RigidBody rb(1.0f, scene.getActiveCamera()->cameraPos);
	rb.transferEnergy(100.0f, scene.getActiveCamera()->cameraFront);
	rb.applyAcceleration(Environment::gravitationalAcceleration);
	launchObjects.instances.push_back(rb);
}

void processInput(double dt) {
	scene.ProcessInput(dt);

	// update flash light
	if (States::isActive(&scene.activeSpotLights, 0)) {
		scene.spotLights[0]->position = scene.getActiveCamera()->cameraPos;
		scene.spotLights[0]->direction = scene.getActiveCamera()->cameraFront;
	}

	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		scene.setShouldClose(true);
	}

	if (Keyboard::keyWentUp(GLFW_KEY_L)) {
		States::Toggle(&scene.activeSpotLights, 0); // toggle spot light
	}

	if (Keyboard::keyWentDown(GLFW_KEY_F)) {
		std::cout << "throw ball" << std::endl;
		launchItem(dt);
	}

	for (int i = 0; i < 4; i++) {
		if (Keyboard::keyWentDown(GLFW_KEY_1 + i)) {
			States::Toggle(&scene.activePointLights, i);
		}
	}
}