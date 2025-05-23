#include"mesh.h"

#include<iostream>

std::vector<struct Vertex> Vertex::genList(float* vertices, int noVertices)
{
	std::vector<Vertex> ref(noVertices);

	int stride = sizeof(Vertex) / sizeof(float);

	for (int i = 0; i < noVertices; i++)
	{
		ref[i].pos = glm::vec3(vertices[i * stride + 0], vertices[i * stride + 1], vertices[i * stride + 2]);

		ref[i].normal = glm::vec3(vertices[i * stride + 3], vertices[i * stride + 4], vertices[i * stride + 5]);

		ref[i].texCoord = glm::vec2(vertices[i * stride + 6], vertices[i * stride + 7]);
	}

	return ref;
}


Mesh::Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	:br(br), vertices(vertices), indices(indices), textures(textures), noTex(false)
{
	Setup();
}

Mesh::Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular)
	:br(br), vertices(vertices), indices(indices), diffuse(diffuse), specular(specular), noTex(true)
{
	Setup();
}

void Mesh::Render(Shader shader, glm::vec3 pos, glm::vec3 size, Box* box, bool doRender)
{
	if (noTex)
	{
		// materials
		shader.Set4Float("material.diffuse", diffuse);
		shader.Set4Float("material.specular", specular);
		shader.SetInt("noTex", 1);
	}
	else if(!noTex)
	{
		//textures
		unsigned int diffuseIdx = 0;
		unsigned int specularIdx = 0;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//activate texture
			glActiveTexture(GL_TEXTURE0 + i);

			//retrieve texture info
			std::string name;
			switch (textures[i].type)
			{
			case aiTextureType_DIFFUSE:
				name = "diffuse" + std::to_string(diffuseIdx++);
				break;
			case aiTextureType_SPECULAR:
				name = "specular" + std::to_string(specularIdx++);
				break;
			}

			//set the shader uniform value
			shader.SetInt(name, i);
			// bind texture
			textures[i].Bind();
		}
	}
	if (doRender)
	{
		box->addInstance(br, pos, size);
	
		Nvao.Bind();
		Nvao.draw(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		ArrayObject::Clear();

		// reset texture
		glActiveTexture(GL_TEXTURE0);
	}

}

void Mesh::Setup()
{
	//bind VAO
	Nvao.Generate();
	Nvao.Bind();

	// Generate / set EBO
	Nvao["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
	Nvao["EBO"].Generate();
	Nvao["EBO"].Bind();
	Nvao["EBO"].setData<GLuint>(indices.size(), &indices[0], GL_STATIC_DRAW);

	//generate /set vbo
	Nvao["VBO"] = BufferObject(GL_ARRAY_BUFFER);
	Nvao["VBO"].Generate();
	Nvao["VBO"].Bind();
	Nvao["VBO"].setData<Vertex>(vertices.size()	, &vertices[0], GL_STATIC_DRAW);

	//set vertex attribute pointers
	//vertex positions
	Nvao["VBO"].SetAttPointer<GLfloat>(0, 3, GL_FLOAT, 8, 0);
	//normal ray
	Nvao["VBO"].SetAttPointer<GLfloat>(1, 3, GL_FLOAT, 8, 3);
	//texture coordinates
	Nvao["VBO"].SetAttPointer<GLfloat>(2, 2, GL_FLOAT, 8, 6);

	Nvao["VBO"].Clear();

	ArrayObject::Clear();
}

void Mesh::Cleanup()
{
	Nvao.Cleanup();
}