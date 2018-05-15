#pragma once
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>
using namespace std;
class Model
{
public:
	Model(string path);

	void Draw(Shader shader);

	~Model();

private:
	//模型数据
	vector<Mesh> meshes;
	string directory;

	//加载过的纹理
	vector<Texture> textures_loaded;

	//函数
	void loadModel(string path);

	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

	unsigned int TextureFromFile(string fileName, string directory);
};
