#include"Scene.h"


unsigned int Scene::scrWidth = 0;
unsigned int Scene::scrHeight = 0;


/*
	callbacks
*/

//window resize
void Scene::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Scene::scrHeight = height;
	Scene::scrWidth = width;
}
/*
	constructor
*/

Scene::Scene(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int scrWidth, unsigned int srcHeight)
	:glfwVersionMajor(glfwVersionMajor), glfwVersionMinor(glfwVersionMinor), title(title), activeCamera(-1), activePointLights(0), activeSpotLights(0)
{
	Scene::scrWidth = scrWidth;
	Scene::scrHeight = srcHeight;

	setWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
}

/*
	Initialization
*/

bool Scene::init()
{
	glfwInit();

	//set version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OEPNGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//intialize window
	window = glfwCreateWindow(scrWidth, scrHeight, title, NULL, NULL);
	if (window == NULL)
	{
		// not created
		return false;
	}
	glfwMakeContextCurrent(window);

	//set glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize Glad" << std::endl;
		glfwTerminate();
		return false;
	}

	//setup screen
	glViewport(0, 0, scrWidth, scrHeight);

	/*
		callBacks;
	*/

	// frame size
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	//key pressed
	glfwSetKeyCallback(window, Keyboard::keyCallback);
	//cursor moved
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	// mouse Button Pressed
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	//mouse scroll
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	/*
		set Rendering parameters
	*/
	glEnable(GL_DEPTH_TEST); // doesnt show vertices not visible to camera (back of object)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

/*
	main loop methods
*/

//process input
void Scene::ProcessInput(float dt)
{
	if (activeCamera != -1 && cameras.size())
	{
		//active camera exist

		// set camera direction
		cameras[activeCamera]->UpdateCameraDirection(Mouse::getDX(), Mouse::getDY());


		// set camera zoom
		cameras[activeCamera]->UpdateCameraZoom(Mouse::getScrollDY());

		if (Keyboard::key(GLFW_KEY_W))
		{
			cameras[activeCamera]->UpdateCameraPos(CameraDirection::FORWARD, dt);
		}
		if (Keyboard::key(GLFW_KEY_S))
		{
			cameras[activeCamera]->UpdateCameraPos(CameraDirection::BACKWARD, dt);
		}
		if (Keyboard::key(GLFW_KEY_A))
		{
			cameras[activeCamera]->UpdateCameraPos(CameraDirection::LEFT, dt);
		}
		if (Keyboard::key(GLFW_KEY_D))
		{
			cameras[activeCamera]->UpdateCameraPos(CameraDirection::RIGHT, dt);
		}
		if (Keyboard::key(GLFW_KEY_LEFT_CONTROL))
		{
			cameras[activeCamera]->UpdateCameraPos(CameraDirection::DOWN, dt);
		}
		if (Keyboard::key(GLFW_KEY_SPACE))
		{
			cameras[activeCamera]->UpdateCameraPos(CameraDirection::UP, dt);
		}

		// set matrices
		view = cameras[activeCamera]->getViewMatrix();
		projection = glm::perspective(glm::radians(cameras[activeCamera]->zoom), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);

		//set pos at end
		cameraPos = cameras[activeCamera]->cameraPos;
	}
}

// update screen before each frame
void Scene::Update()
{
	glClearColor(bg[0], bg[1], bg[2], bg[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// update Screen after frame
void Scene::NewFrame()
{
	//send new frame window
	glfwSwapBuffers(window);
	glfwPollEvents();
}

//set uniform shader variables (lighting, etc)
void Scene::Render(Shader shader, bool applyLighting)
{
	//activate shader
	shader.Activate();

	//set camera values
	shader.SetMat4("view", view);
	shader.SetMat4("projection", projection);
	shader.Set3Float("viewPos", cameraPos);

	// lighting
	if (applyLighting)
	{
		// point lights
		unsigned int nbLights = pointLights.size();
		unsigned int nbActiveLights = 0;
		for (unsigned int i = 0; i < nbLights; i++)
		{
			if (States::isActive(&activePointLights, i))
			{
				// number of active lights
				pointLights[i]->render(shader, nbActiveLights);
				nbActiveLights++;
			}
		}
		shader.SetInt("nbPointLights", nbActiveLights);

		// spot lights
		nbLights = spotLights.size();
		nbActiveLights = 0;
		for (unsigned int i = 0; i < nbLights; i++)
		{
			if (States::isActive(&activeSpotLights, i))
			{
				// number of active lights
				spotLights[i]->render(shader, nbActiveLights);
				nbActiveLights++;
			}
		}
		shader.SetInt("nbSpotLights", nbActiveLights);

		// Directional Light
		dirLight->render(shader);
	}
}

/*
	cleanup method
*/

void Scene::Cleanup()
{
	glfwTerminate();
}

/*
	accessors
*/

bool Scene::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

Camera* Scene::getActiveCamera() 
{
	return (activeCamera >= 0 && activeCamera < cameras.size()) ? cameras[activeCamera] : nullptr;
}

/*
	modifiers
*/

void Scene::setShouldClose(bool shouldClose) 
{
	glfwSetWindowShouldClose(window, shouldClose);
}

void Scene::setWindowColor(float r, float g, float b, float a) {
	bg[0] = r;
	bg[1] = g;
	bg[2] = b;
	bg[3] = a;
}