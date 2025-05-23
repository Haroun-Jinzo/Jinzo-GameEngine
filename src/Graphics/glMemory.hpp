#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<map>

/*
	class for buffers 
	-VBO, EBO, etc
*/

class BufferObject
{
public:
	//value / location
	GLuint val;
	// type buffer (Array buffer or array element buffer, etc)
	GLenum type;

	BufferObject() {}

	BufferObject(GLenum type)
		:type(type) {}

	//generate object
	void Generate()
	{
		glGenBuffers(1, &val);
	}

	//bind object
	void Bind()
	{
		glBindBuffer(type, val);
	}

	//set data
	template<typename T>
	void setData(GLuint nbElements, T* data, GLenum usage)
	{
		glBufferData(type, nbElements * sizeof(T), data, usage);
	}

	//update data (glBuffersubdata)
	template<typename T>
	void UpdateData(GLuint offset, GLuint nbElements, T* data)
	{
		glBufferSubData(type, offset, nbElements * sizeof(T), data);
	
	}

	//set attributes pointers
	template<typename T>
	void SetAttPointer(GLuint index, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0)
	{
		glVertexAttribPointer(index, size, type, GL_FALSE, stride * sizeof(T), (void*)(offset * sizeof(T)));
		glEnableVertexAttribArray(index);
		if (divisor > 0)
		{
			//reset index attribute every divisor iteration (instanceing)
			glVertexAttribDivisor(index, divisor);
		}
	}

	//clear buffer objects (bind 0)
	void Clear()
	{
		glBindBuffer(type, 0);
	}

	//cleanup
	void Cleanup()
	{
		glDeleteBuffers(1, &val);
	}
};

/*
	class for array objects
	-VAO,
*/

class ArrayObject
{
public:
	//value / location
	GLuint val;

	//map of names to buffers
	std::map<const char*, BufferObject> buffers;

	// get Buffer
	BufferObject& operator[](const char* key)
	{
		return buffers[key];
	}

	//generate object
	void Generate()
	{
		glGenVertexArrays(1, &val);
	}

	//bind
	void Bind()
	{
		glBindVertexArray(val);
	}

	void draw(GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1)
	{
		glDrawElementsInstanced(mode, count, type, (void*)indices, instanceCount);
	}

	//clanup
	void Cleanup()
	{
		glDeleteVertexArrays(1, &val);

		for (auto& pair : buffers)
		{
			pair.second.Cleanup();
		}
	}

	//clear array object (bind 0)
	static void Clear()
	{
		glBindVertexArray(0);
	}
};