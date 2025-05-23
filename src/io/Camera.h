#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	glm::vec3 cameraPos;

	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	glm::vec3 wordlUp;

	float yaw;
	float pitch;
	float speed;
	float zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f));

	void UpdateCameraDirection(double dx, double dy);
	void UpdateCameraPos(CameraDirection dir, double dt);
	void UpdateCameraZoom(double dy);

	glm::mat4 getViewMatrix();

private:
	void UpdateCameraVectors();
};