#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Screen
{
public:
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

	Screen();

	bool init();

	void SetParameters();

	//main loop

	void Update();
	void Begin();

	//window closing accessor and modifier
	bool ShouldClose();
	void SetShouldClose(bool souldClose);

	GLFWwindow* window;
};