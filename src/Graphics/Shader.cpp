#include"Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	Generate(vertexShaderPath, fragmentShaderPath);
}

void Shader::Generate(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	int success;
	char infoLog[512];

	GLuint vertexShader = CompileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragShader = CompileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Linking error" << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void Shader::Activate()
{
	glUseProgram(id);
}

std::string Shader::LoadShaderSrc(const char* filename)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "could not open " << filename << std::endl;
	}

	file.close();

	return ret;
}

GLuint Shader::CompileShader(const char* filepath, GLenum type)
{
	int success;
	char infoLog[512];

	GLuint shaderValues = glCreateShader(type);
	std::string shaderSrc = LoadShaderSrc(filepath);
	const GLchar* shader = shaderSrc.c_str();
	glShaderSource(shaderValues, 1, &shader, NULL);
	glCompileShader(shaderValues);

	//catch error
	glGetShaderiv(shaderValues, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderValues, 512, NULL, infoLog);
		std::cout << "Error with vertex shader comp: " << std::endl << infoLog << std::endl;
	}

	return shaderValues;
}

void Shader::SetMat4(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::Set3Float(const std::string& name, glm::vec3 v)
{
	Set3Float(name, v.x, v.y, v.z);
}

void Shader::Set3Float(const std::string& name, float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}

void Shader::Set4Float(const std::string& name, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

void Shader::Set4Float(const std::string& name, aiColor4D color)
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), color.r, color.g, color.b, color.a);
}

void Shader::Set4Float(const std::string& name, glm::vec4 v)
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}