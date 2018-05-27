#include "MeshLoader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneNode.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Logger.h"
#include "TextureLoader.h"
#include "ResourceLoader.h"
#include "Scene.h"

std::vector<Mesh*> MeshLoader::meshStore = std::vector<Mesh*>();
std::map<string, Mesh*> MeshLoader::meshMap = std::map<string, Mesh*>();
std::map<string, Material*> MeshLoader::matMap = std::map<string, Material*>();
// --------------------------------------------------------------------------------------------
void MeshLoader::Clean()
{
	for (unsigned int i = 0; i < MeshLoader::meshStore.size(); ++i)
	{
		delete MeshLoader::meshStore[i];
	}
}
// --------------------------------------------------------------------------------------------
SceneNode* MeshLoader::LoadMesh(std::string path, bool setDefaultMaterial)
{
	Log::Message("Loading mesh file at: " + path + ".", LOG_INIT);

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::Message("Assimp failed to load model at path: " + path, LOG_ERROR);
		return nullptr;
	}

	std::string directory = path.substr(0, path.find_last_of("/"));

	Log::Message("Succesfully loaded: " + path + ".", LOG_INIT);

	SceneNode* node = MeshLoader::processNode(scene->mRootNode, scene, directory, setDefaultMaterial);

	return node;
}
// --------------------------------------------------------------------------------------------
SceneNode* MeshLoader::processNode(aiNode* aNode, const aiScene* aScene, std::string directory, bool setDefaultMaterial)
{
	SceneNode* node = new SceneNode();

	//	string id = std::string(aNode->mName.C_Str()) + directory;
	//	if (MeshLoader::meshMap.find(id) != MeshLoader::meshMap.end())
	//	{
	//		if (aNode->mNumMeshes == 1)
	//		{
	//			node->mesh = (MeshLoader::meshMap[id]);
	//			node->material = (MeshLoader::matMap[id]);
	//		}
	//		else
	//		{
	//			SceneNode* child = new SceneNode(node);
	//			node->mesh = (MeshLoader::meshMap[id]);
	//			node->material = (MeshLoader::matMap[id]);
	//			node->AddChild(child);
	//		}
	//		return node;
	//	}

	for (unsigned int i = 0; i < aNode->mNumMeshes; ++i)
	{
		glm::vec3 boxMin, boxMax;
		aiMesh*     assimpMesh = aScene->mMeshes[aNode->mMeshes[i]];
		aiMaterial* assimpMat = aScene->mMaterials[assimpMesh->mMaterialIndex];
		Mesh*       mesh = MeshLoader::parseMesh(assimpMesh, aScene, boxMin, boxMax);
		Material*   material = nullptr;
		if (setDefaultMaterial)
		{
			material = MeshLoader::parseMaterial(assimpMat, aScene, directory);
		}

		if (aNode->mNumMeshes == 1)
		{
			node->mesh = mesh;
			if (setDefaultMaterial)
			{
				node->material = material;
			}
			node->BoxMin = boxMin;
			node->BoxMax = boxMax;
		}
		else
		{
			SceneNode* child = new SceneNode(node);
			child->mesh = mesh;
			child->material = material;
			child->BoxMin = boxMin;
			child->BoxMax = boxMax;
			node->AddChild(child);
		}
	}

	for (unsigned int i = 0; i < aNode->mNumChildren; ++i)
	{
		node->AddChild(MeshLoader::processNode(aNode->mChildren[i], aScene, directory, setDefaultMaterial));
	}

	//	MeshLoader::meshMap[id] = node->mesh;
	//	MeshLoader::matMap[id] = node->material;

	return node;
}
// --------------------------------------------------------------------------------------------
Mesh* MeshLoader::parseMesh(aiMesh* aMesh, const aiScene* aScene, glm::vec3& out_Min, glm::vec3& out_Max)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<unsigned int> indices;

	positions.resize(aMesh->mNumVertices);
	normals.resize(aMesh->mNumVertices);
	if (aMesh->mNumUVComponents > 0)
	{
		uv.resize(aMesh->mNumVertices);
		tangents.resize(aMesh->mNumVertices);
		bitangents.resize(aMesh->mNumVertices);
	}
	indices.resize(aMesh->mNumFaces * 3);

	glm::vec3 pMin(99999.0);
	glm::vec3 pMax(-99999.0);

	for (unsigned int i = 0; i < aMesh->mNumVertices; ++i)
	{
		positions[i] = glm::vec3(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z);
		if (aMesh->mNormals)
		{
			normals[i] = glm::vec3(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z);
		}
		if (aMesh->mTextureCoords)
		{
			uv[i] = glm::vec2(aMesh->mTextureCoords[0][i].x, aMesh->mTextureCoords[0][i].y);
		}
		if (aMesh->mTangents)
		{
			tangents[i] = glm::vec3(aMesh->mTangents[i].x, aMesh->mTangents[i].y, aMesh->mTangents[i].z);
			bitangents[i] = glm::vec3(aMesh->mBitangents[i].x, aMesh->mBitangents[i].y, aMesh->mBitangents[i].z);
		}
		if (positions[i].x < pMin.x) pMin.x = positions[i].x;
		if (positions[i].y < pMin.y) pMin.y = positions[i].y;
		if (positions[i].z < pMin.z) pMin.z = positions[i].z;
		if (positions[i].x > pMax.x) pMax.x = positions[i].x;
		if (positions[i].y > pMax.y) pMax.y = positions[i].y;
		if (positions[i].z > pMax.z) pMax.z = positions[i].z;
	}
	for (unsigned int f = 0; f < aMesh->mNumFaces; ++f)
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			indices[f * 3 + i] = aMesh->mFaces[f].mIndices[i];
		}
	}

	Mesh *mesh = new Mesh;
	mesh->Positions = positions;
	mesh->UV = uv;
	mesh->Normals = normals;
	mesh->Tangents = tangents;
	mesh->Bitangents = bitangents;
	mesh->Indices = indices;
	mesh->Topology = TRIANGLES;

	mesh->Name = std::string(aMesh->mName.C_Str());

	mesh->Finalize(true);

	out_Min.x = pMin.x;
	out_Min.y = pMin.y;
	out_Min.z = pMin.z;
	out_Max.x = pMax.x;
	out_Max.y = pMax.y;
	out_Max.z = pMax.z;

	MeshLoader::meshStore.push_back(mesh);

	return mesh;
}
// --------------------------------------------------------------------------------------------
Material *MeshLoader::parseMaterial(aiMaterial* aMaterial, const aiScene* aScene, std::string directory)
{
	Material* material;
	aiString file;
	aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
	std::string diffPath = std::string(file.C_Str());
	bool alpha = false;
	if (diffPath.find("_alpha") != std::string::npos)
	{
		material = Scene::GetDefaultMaterialCopy();
		//		alpha = true;
	}
	else  // else, make default deferred material
	{
		material = Scene::GetDefaultMaterialCopy();
	}

	if (aMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString file;
		aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		std::string fileName = MeshLoader::processPath(&file, directory);
		Texture* texture = ResourceLoader::LoadTexture(fileName, fileName, GL_TEXTURE_2D, alpha ? GL_RGBA : GL_RGB, true);
		if (texture)
		{
			material->SetTexture("TexAlbedo", texture, 0);
		}
	}
	if (aMaterial->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
	{
		aiString file;
		aMaterial->GetTexture(aiTextureType_DISPLACEMENT, 0, &file);
		std::string fileName = MeshLoader::processPath(&file, directory);

		Texture* texture = ResourceLoader::LoadTexture(fileName, fileName);
		if (texture)
		{
			material->SetTexture("TexNormal", texture, 1);
		}
	}
	if (aMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
	{
		aiString file;
		aMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		std::string fileName = MeshLoader::processPath(&file, directory);

		Texture* texture = ResourceLoader::LoadTexture(fileName, fileName);
		if (texture)
		{
			material->SetTexture("TexMetallic", texture, 2);
		}
	}
	if (aMaterial->GetTextureCount(aiTextureType_SHININESS) > 0)
	{
		aiString file;
		aMaterial->GetTexture(aiTextureType_SHININESS, 0, &file);
		std::string fileName = MeshLoader::processPath(&file, directory);

		Texture* texture = ResourceLoader::LoadTexture(fileName, fileName);
		if (texture)
		{
			material->SetTexture("TexRoughness", texture, 3);
		}
	}
	if (aMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0)
	{
		aiString file;
		aMaterial->GetTexture(aiTextureType_AMBIENT, 0, &file);
		std::string fileName = MeshLoader::processPath(&file, directory);

		Texture* texture = ResourceLoader::LoadTexture(fileName, fileName);
		if (texture)
		{
			material->SetTexture("TexAO", texture, 4);
		}
	}

	return material;
}
// --------------------------------------------------------------------------------------------
std::string MeshLoader::processPath(aiString *aPath, std::string directory)
{
	std::string path = std::string(aPath->C_Str());

	if (path.find(":/") == std::string::npos || path.find(":\\") == std::string::npos)
		path = directory + "/" + path;
	return path;
}