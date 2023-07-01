#pragma once
#include <iostream>
#include <vector>
#include <glm.hpp>

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct mTexture {
	unsigned int id;
	string type;
	string path;
};

class Shader;
class Mesh
{
public:
	/* 网格数据 */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<mTexture> textures;

	/* 函数 */
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<mTexture> textures);
	void Draw(Shader shader);

private:
	/// <summary>
	/// 渲染数据
	/// </summary>
	unsigned int VAO, VBO, EBO;

	/// <summary>
	/// 初始化网格
	/// </summary>
	void setupMesh();
};