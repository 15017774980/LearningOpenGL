#pragma once
#include <vector>
#include <string>
#include <assimp/scene.h>
#include "../Mesh/Mesh.h"

using namespace std;

struct mTexture;
class Shader;
class Model
{
public:
	Model( string const & path, bool gamma = false);
	void Draw(Shader shader);

	vector<Mesh> meshs;

private:
	// 模型数据
	string directory;
	vector<mTexture> textures_loaded;
	bool gammaCorrection;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<mTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};