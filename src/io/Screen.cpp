#include"Screen.h"
#include"Keyboard.h"
#include"Mouse.h"

unsigned int Screen::SCR_WIDTH = 800;
unsigned int Screen::SCR_HEIGHT = 600;

void Screen::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

Screen::Screen():
	window(nullptr) {}

bool Screen::init()
{
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Jinzo Engine 1.0", NULL, NULL);	
	if (!window)
	{
		//window not created
		return false;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	return true;
}

void Screen::SetParameters()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
}

void Screen::Update()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::Begin()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Screen::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Screen::SetShouldClose(bool shouldClose)
{
	glfwSetWindowShouldClose(window, shouldClose);
}