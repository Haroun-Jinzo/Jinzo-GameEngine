#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include<vector>

#include"mesh.h"

#include"models/Box.hpp"

#include"../Physics/RigidBody.h"
#include"../Physics/Environment.h"

#include"../Algorithms/Bounds.h"

class Model
{
public:
	RigidBody rb;
	glm::vec3 size;

	BoundTypes boundType;

	Model(BoundTypes boundType = BoundTypes::AABB, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

	void init();
	void loadModel(std::string path);

	void Render(Shader shader, float dt,Box* box, bool setModel = true, bool doRender = true);

	std::vector<Mesh> meshes;

	void CleanUP();

protected:
	bool noTex;

	std::string directory;

	std::vector<Texture> textures_loaded;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type);
};