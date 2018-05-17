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
	static void Clear();

	static SceneNode* MakeSceneNode();

	static SceneNode* MakeSceneNode(Mesh* mesh, Material* material);
	static SceneNode* MakeSceneNode(SceneNode* node);

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

public:
	static std::vector<DirectionalLight*> m_DirectionalLights;
	static std::vector<PointLight*>       m_PointLights;
	static std::vector<SpotLight*>       m_SpotLights;

	static Material* defaultMaterial;
};
