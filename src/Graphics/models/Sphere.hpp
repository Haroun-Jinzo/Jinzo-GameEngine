#pragma once

#include"../Model.h"
#include"ModelArray.hpp"

class Sphere : public Model
{
public:
	Sphere(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f))
		: Model(BoundTypes::SPHERE, pos, size, true)
	{
		rb.mass = 1.0f;
	}

	void init()
	{
		loadModel("assets/Models/sphere/scene.gltf");
	}
};

class SphereArray : public ModelArray<Sphere>
{
public:
	void init()
	{
		model = Sphere(glm::vec3(0.0f), glm::vec3(0.25f));
		ModelArray::init();
	}
};