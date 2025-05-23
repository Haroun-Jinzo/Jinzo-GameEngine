#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<vector>

#include<glm/glm.hpp>

#include"Graphics/light.h"
#include"Graphics/Shader.h"

#include"io/Camera.h"
#include"io/Mouse.h"
#include"io/Keyboard.h"

#include"Algorithms/States.hpp"

class Scene
{
public:
	/*
		callbacks
	*/

	//window resize
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	/*
		constructor
	*/

	Scene() {};
	Scene(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int scrWidth, unsigned int srcHeight);

	/*
		Initialization
	*/

	bool init();

	/*
		main loop methods
	*/

	//process input
	void ProcessInput(float dt);

	// update screen before each frame
	void Update();

	// update Screen after frame
	void NewFrame();

	//set uniform shader variables (lighting, etc)
	void Render(Shader shader, bool applyLighting = true);

	/*
		cleanup method
	*/

	void Cleanup();

	/*
		accessors
	*/

	bool ShouldClose();

	Camera* getActiveCamera();

	/*
		modifiers
	*/

	void setShouldClose(bool shouldClose);

	void setWindowColor(float r, float g, float b, float a);

	/*
		// list of point lights
	*/
	std::vector<PointLight*> pointLights;
	unsigned int activePointLights;
	// list of spot lights
	std::vector<SpotLight*> spotLights;
	unsigned int activeSpotLights;
	//direction light
	DirLight* dirLight;
	bool dirLightActive;
	/*
		Camera
	*/
	std::vector<Camera*> cameras;
	unsigned int activeCamera;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPos;

protected:
	 //window object
	GLFWwindow* window;

	//window vals
	const char* title;
	static unsigned int scrWidth;
	static unsigned int scrHeight;

	float bg[4]; //backgroound color

	// GLFW info
	int glfwVersionMajor;
	int glfwVersionMinor;

};
