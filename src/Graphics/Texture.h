#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<assimp/scene.h>

#include<stb/stb_image.h>

class Texture
{
public:
	Texture();
	Texture(std::string dir, std::string path, aiTextureType type);

	void Generate();
	void load(bool flip = true);

	void Bind();
	void Unbind();
	void Delete();

	unsigned int id;
	aiTextureType type;
	std::string dir;
	std::string path; 
};