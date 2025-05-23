#pragma once

#include"../Model.h"

#include"box.hpp"

#define UPPER_BOUND 100

template <class T>
class ModelArray
{
public:
	std::vector<RigidBody> instances;

	void init()
	{
		model.init();

		// generate positions VBO
		posVBO = BufferObject(GL_ARRAY_BUFFER);
		posVBO.Generate();
		posVBO.Bind();
		posVBO.setData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);

		// generate size VBO
		sizeVBO = BufferObject(GL_ARRAY_BUFFER);
		sizeVBO.Generate();
		sizeVBO.Bind();
		sizeVBO.setData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);


		// set attribute pointers for each mesh
		for (unsigned int i = 0, size = model.meshes.size(); i < size; i++) 
		{
			model.meshes[i].Nvao.Bind();

			// set vertex attrib pointers
			// positions
			posVBO.Bind();
			posVBO.SetAttPointer<glm::vec3>(3, 3, GL_FLOAT, 1, 0, 1);
			//size
			sizeVBO.Bind();
			sizeVBO.SetAttPointer<glm::vec3>(4, 3, GL_FLOAT, 1, 0, 1);

			ArrayObject::Clear();
		}
	}

	void Render(Shader shader, float dt,Box* box, bool setLists = true) {
		if (setLists) {
			positions.clear();
			sizes.clear();

			for (RigidBody& rb : instances) {
				rb.update(dt);
				positions.push_back(rb.pos);
				sizes.push_back(model.size);
			}
		}

		shader.SetMat4("model", glm::mat4(1.0f));

		model.Render(shader, dt, nullptr, false, false);

		int instances = std::min(UPPER_BOUND, (int)positions.size()); // if more than 100 instances, only render 100

		// update data
		if (instances != 0) {
			
			// if instances exist

			//reset VBO Data
			posVBO.Bind();
			posVBO.UpdateData<glm::vec3>(0, instances, &positions[0]);

			sizeVBO.Bind();
			sizeVBO.UpdateData<glm::vec3>(0, instances, &sizes[0]);

			sizeVBO.Clear();
		}

		// render instanced data
		for (unsigned int i = 0, length = model.meshes.size(); i < length; i++) {
			for (unsigned int j = 0; j < instances; j++) {
				box->addInstance(model.meshes[i].br, positions[j], sizes[j]);
			}

			model.meshes[i].Nvao.Bind();
			model.meshes[i].Nvao.draw(GL_TRIANGLES, model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, instances);
			ArrayObject::Clear();
		}
	}
	void setSize(glm::vec3 size)
	{
		model.size = size;
	}

	void CleanUp()
	{
		model.CleanUP();
		posVBO.Cleanup();
		sizeVBO.Cleanup();
	}

protected:
	T model;

	BufferObject posVBO;
	BufferObject sizeVBO;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> sizes;
};