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
	//ģ������
	vector<Mesh> meshes;
	string directory;

	//���ع�������
	vector<Texture> textures_loaded;

	//����
	void loadModel(string path);

	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

	unsigned int TextureFromFile(string fileName, string directory);
};
