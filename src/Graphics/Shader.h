#pragma once

#include<glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include<assimp/scene.h>

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int id;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void Generate(const char* vertexShaderPath, const char* fragShaderPath);

	void Activate();

	//utility functions
	std::string LoadShaderSrc(const char* filepath);
	GLuint CompileShader(const char* filepath, GLenum type);

	//uniform functions
	void SetMat4(const std::string& name, glm::mat4 value);
	void SetFloat(const std::string& name, float value);
	void Set3Float(const std::string& name, glm::vec3 v);
	void Set3Float(const std::string& name, float v1, float v2, float v3);
	void Set4Float(const std::string& name, float v1, float v2, float v3, float v4);
	void Set4Float(const std::string& name, aiColor4D color);
	void Set4Float(const std::string& name, glm::vec4 v);
	void SetInt(const std::string& name, int value);
};