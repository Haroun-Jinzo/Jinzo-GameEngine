#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<vector>
#include<glm/glm.hpp>

#include"Shader.h"
#include"Texture.h"
#include"glMemory.hpp"

#include"models/Box.hpp"

#include"../Algorithms/Bounds.h"


struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;

	/*
	v1.vec3.x|v1.vec3.y|v1.vec3.z|v1.vec2.x|v1.vec2.y|
	v2.vec3.x|v2.vec3.y|v2.vec3.z|v2.vec2.x|v2.vec2.y
	*/

	static std::vector<Vertex> genList(float* vertices, int noVertices);
};


class Mesh
{
public:
	BoundingRegion br;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	ArrayObject Nvao;

	std::vector<Texture> textures;
	aiColor4D diffuse;
	aiColor4D specular;

	Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures = {});
	Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular);

	void Render(Shader shader, glm::vec3 pos, glm::vec3 size, Box* box, bool doRender = true);

	void Cleanup();

private:
	unsigned int VBO, EBO;

	bool noTex;

	void Setup();
};