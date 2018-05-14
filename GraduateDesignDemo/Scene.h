#pragma once
#include "Camera.h"
#include "TextureCube.h"
#include "Shader.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <irrklang/irrKlang.h>
using namespace irrklang;

class Mesh;
class Material;
class SceneNode;

class Scene
{
public:
	static Camera* mainCamera;

	static ISoundEngine *SoundPlayer;

	static SceneNode* Root;
	//	static unsigned int CounterID;

public:
	// clears all scene nodes currently part of the scene.
	static void Clear();

	// static helper function that directly builds an empty scene node. Other sub-engines can
	// directly add children to this empty scene node (w/ identity matrix as transform).
	static SceneNode* MakeSceneNode();
	//  similar to the MakeScene function, but directly builds a node w/ attached mesh and
	// material.
	static SceneNode* MakeSceneNode(Mesh* mesh, Material* material);
	// because the scene manages we have to copy the nodes from here.
	static SceneNode* MakeSceneNode(SceneNode* node);

	// deletes a scene node from the global scene hierarchy (together with its  children).
	static void DeleteSceneNode(SceneNode* node);

	static void AddChild(SceneNode* node);

	static void Render();

	static void SetSkyBox(TextureCube* Tex);

	static void Init();

	static void Update();

	static void AddLight(DirectionalLight *light);
	static void AddLight(PointLight       *light);
	static void AddLight(SpotLight       *light);

	static Material* GetDefaultMaterialCopy();

	static void PrintNodeTree(SceneNode* root, int depth = 0);

private:
	static void UpdateUboData();

	static void UpdateDefaultMaterial();

	static void render(SceneNode *node);

	static SceneNode* skyboxSceneNode;
	static Material* skyBoxMaterial;
	static TextureCube* skyBoxTex;

	static GLuint UBO;

	static std::vector<DirectionalLight*> m_DirectionalLights;
	static std::vector<PointLight*>       m_PointLights;
	static std::vector<SpotLight*>       m_SpotLights;

	static Material* defaultMaterial;
};
