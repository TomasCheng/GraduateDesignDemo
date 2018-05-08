#include "Scene.h"
#include "SceneNode.h"
#include <stack>
#include "CubeMesh.h"
#include "ResourceLoader.h"
#include "iostream"
#include "TextRender.h"
#include "SphereMesh.h"

SceneNode* Scene::Root = new SceneNode(nullptr);
//unsigned int Scene::CounterID = 0;
SceneNode* Scene::skyboxSceneNode = nullptr;
Material* Scene::skyBoxMaterial = nullptr;
TextureCube* Scene::skyBoxTex = nullptr;
Camera* Scene::mainCamera = nullptr;
GLuint Scene::UBO = 0;
Material* Scene::defaultMaterial = nullptr;
std::vector<DirectionalLight*> Scene::m_DirectionalLights = vector<DirectionalLight*>();
std::vector<PointLight*>       Scene::m_PointLights = vector<PointLight*>();
std::vector<SpotLight*>       Scene::m_SpotLights = vector<SpotLight*>();
ISoundEngine* Scene::SoundPlayer = nullptr;
// --------------------------------------------------------------------------------------------
void Scene::Clear()
{
	Scene::DeleteSceneNode(Root);
	Scene::Root = new SceneNode();
}
// --------------------------------------------------------------------------------------------
SceneNode* Scene::MakeSceneNode()
{
	SceneNode* node = new SceneNode();
	// keep a global rerefence to this scene node s.t.we can clear the scene's nodes for memory
	// management: end of program or when switching scenes.
//	Root->AddChild(node);
	return node;
}
// --------------------------------------------------------------------------------------------
SceneNode* Scene::MakeSceneNode(Mesh* mesh, Material* material)
{
	SceneNode* node = new SceneNode();

	node->mesh = mesh;
	node->material = material;

	// keep a global rerefence to this scene node s.t. we can clear the scene's nodes for
	// memory management: end of program or when switching scenes.
//	Root->AddChild(node);
	return node;
}
// --------------------------------------------------------------------------------------------
SceneNode* Scene::MakeSceneNode(SceneNode* node)
{
	//	SceneNode *newNode = new SceneNode(Scene::CounterID++);
	//
	//	newNode->mesh = node->mesh;
	//	newNode->material = node->material;
	//	newNode->BoxMin = node->BoxMin;
	//	newNode->BoxMax = node->BoxMax;
	//
	//	// traverse through the list of children and add them correspondingly
	//	std::stack<SceneNode*> nodeStack;
	//	for (unsigned int i = 0; i < node->GetChildCount(); ++i)
	//		nodeStack.push(node->GetChildByIndex(i));
	//	while (!nodeStack.empty())
	//	{
	//		SceneNode* child = nodeStack.top();
	//		nodeStack.pop();
	//		// similarly, create SceneNode for each child and push to scene node memory list.
	//		SceneNode* newChild = new SceneNode(Scene::CounterID++);
	//		newChild->mesh = child->mesh;
	//		newChild->material = child->material;
	//		newChild->BoxMin = child->BoxMin;
	//		newChild->BoxMax = child->BoxMax;
	//		newNode->AddChild(newChild);
	//
	//		for (unsigned int i = 0; i < child->GetChildCount(); ++i)
	//			nodeStack.push(child->GetChildByIndex(i));
	//	}

	//	Root->AddChild(newNode);
	//	return newNode;

//	Root->AddChild(node);
	return node;
}
// --------------------------------------------------------------------------------------------
void Scene::DeleteSceneNode(SceneNode *node)
{
	if (node->GetParent())
	{
		node->GetParent()->RemoveChild(node->GetID());
	}
	// all delete logic is contained within each scene node's destructor.
	delete node;
}

void Scene::AddChild(SceneNode* node)
{
	if (node != nullptr)
	{
		Scene::Root->AddChild(node);
	}
}

void Scene::Render()
{
	//深度便利場景書渲染每個節點
	if (Scene::Root != nullptr)
	{
		render(Scene::Root);
	}

	if (Scene::skyboxSceneNode != nullptr)
	{
		//渲染天空盒
		Scene::skyBoxMaterial->Use();
		Scene::skyBoxTex->Bind();
		glm::mat4 m(1.0f);
		m = glm::scale(m, glm::vec3(10000));
		glm::mat4 view = glm::mat4(glm::mat3(mainCamera->GetViewMatrix()));
		Scene::skyBoxMaterial->SetMatrix("view", view);
		Scene::skyBoxMaterial->SetMatrix("proj", mainCamera->GetProj());
		Scene::skyBoxMaterial->SetMatrix("model", m);
		Scene::skyboxSceneNode->Render();
	}
}

void Scene::SetSkyBox(TextureCube* Tex)
{
	Scene::skyBoxTex = Tex;

	Shader* skyBoxShader = ResourceLoader::LoadShader("skyBox", "cubeMapShader.vert", "cubeMapShader.frag");
	Scene::skyBoxMaterial = new Material(skyBoxShader);
	Scene::skyBoxMaterial->SetTextureCube("cubemap", Tex);
	Mesh* mesh = new Cube;
	Scene::skyboxSceneNode = new SceneNode(mesh, Scene::skyBoxMaterial, nullptr);
}

void Scene::Init()
{
	//設置著色器中的公共部分 ubo
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2048, nullptr, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);

	//生成默認材質
	Shader* defaultShader = ResourceLoader::LoadShader("default", "DefaultVertShader.vert", "DefaultFragShader.frag");

	unsigned int lights_index = glGetUniformBlockIndex(defaultShader->ID, "Global");
	glUniformBlockBinding(defaultShader->ID, lights_index, 0);

	Scene::defaultMaterial = new Material(defaultShader);
	//	Texture* defaultTexAlbedo = ResourceLoader::LoadTexture("default", "timg.jpg");
	//	Texture* defaultTexMetallic = ResourceLoader::LoadTexture("default", "timg.jpg");
	//	defaultMaterial->SetTexture("TexAlbedo", defaultTexAlbedo, 0);
	//	defaultMaterial->SetTexture("TexMetallic", defaultTexMetallic, 2);
	defaultMaterial->SetVector("MainColor", glm::vec3(1));

	UpdateUboData();

	//初始化音频播放器
	SoundPlayer = createIrrKlangDevice();
}

void Scene::Update()
{
	UpdateUboData();
	//	UpdateDefaultMaterial();
	Render();
}

void Scene::AddLight(DirectionalLight* light)
{
	m_DirectionalLights.push_back(light);
}

void Scene::AddLight(PointLight* light)
{
	m_PointLights.push_back(light);
	if (light->RenderMesh)
	{
		//添加一个球体，颜色为光的颜色
		Mesh* mesh = new Sphere;
		Shader* lightShader = ResourceLoader::LoadShader("PointLight", "DefaultVertShader.vert", "Light.frag");
		Material* mat = new Material(lightShader);
		mat->SetVector("lightColor", light->Color);
		SceneNode* node = new SceneNode(mesh, mat);
		node->SetPosition(light->Position);
		node->SetScale(light->Radius / 4.0);
	}
}

void Scene::AddLight(SpotLight* light)
{
	m_SpotLights.push_back(light);
	if (light->RenderMesh)
	{
		//添加一个球体，颜色为光的颜色
		Mesh* mesh = new Sphere;
		Shader* lightShader = ResourceLoader::LoadShader("PointLight", "DefaultVertShader.vert", "Light.frag");
		Material* mat = new Material(lightShader);
		mat->SetVector("lightColor", light->Color);
		SceneNode* node = new SceneNode(mesh, mat);
		node->SetPosition(light->Position);
		node->SetScale(1.0 / 4.0);
	}
}

Material* Scene::GetDefaultMaterialCopy()
{
	Material defaultMatCopy = defaultMaterial->Copy();
	Material* mat = new Material(defaultMatCopy);
	return mat;

	//	return  defaultMaterial;
}

void Scene::PrintNodeTree(SceneNode* root, int depth)
{
	if (root != nullptr)
	{
		for (int i = 0; i < root->GetChildCount(); i++)
		{
			if (root->GetChildByIndex(i) != nullptr)
			{
				for (int j = 0; j < depth + 1; j++)
				{
					std::cout << "-";
				}
				std::cout << root->GetChildByIndex(i)->GetName();
				std::cout << std::endl;
			}
			PrintNodeTree(root->GetChildByIndex(i), depth + 1);
		}
	}
}

void Scene::UpdateUboData()
{
	//更新ubo中的數據
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	// transformation matrices
	int mat4Size = 64;
	int vec3Size = 16;
	int floatSize = 16;
	int intSize = 4;
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mat4Size, &mainCamera->proj[0][0]); // sizeof(glm::mat4) = 64 bytes
	glm::mat4 view = mainCamera->GetViewMatrix();
	glBufferSubData(GL_UNIFORM_BUFFER, mat4Size, mat4Size, &view[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, mat4Size * 2, vec3Size, &mainCamera->Position[0]);

	// lighting
	int start = mat4Size * 2 + vec3Size;
	for (unsigned int i = 0; i < m_DirectionalLights.size() && i < 4; ++i) // no more than 4 directional lights
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * vec3Size + start, vec3Size, &m_DirectionalLights[i]->Direction[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, i * vec3Size + start + 4 * vec3Size, vec3Size, &(m_DirectionalLights[i]->Color* m_DirectionalLights[i]->Intensity)[0]);
	}
	start += 8 * vec3Size;
	for (unsigned int i = 0; i < m_PointLights.size() && i < 8; ++i) //  constrained to max 8 point lights in forward context
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * vec3Size + start, vec3Size, &m_PointLights[i]->Position[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, i * vec3Size + start + 8 * vec3Size, vec3Size, &(m_PointLights[i]->Color*m_PointLights[i]->Intensity)[0]);
	}
	start += 16 * vec3Size;
	for (unsigned int i = 0; i < m_SpotLights.size() && i < 3; ++i) //
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * vec3Size + start, vec3Size, &m_SpotLights[i]->Position[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, i * vec3Size + start + 3 * vec3Size, vec3Size, &m_SpotLights[i]->Direction[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, i * vec3Size + start + 6 * vec3Size, vec3Size, &(m_SpotLights[i]->Color*m_SpotLights[i]->Intensity)[0]);
	}
	start += 9 * vec3Size;
	for (unsigned int i = 0; i < m_SpotLights.size() && i < 3; ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * floatSize + start, 16, &m_SpotLights[i]->CutOff);
		glBufferSubData(GL_UNIFORM_BUFFER, i * floatSize + start + floatSize * 3, 16, &m_SpotLights[i]->OuterCutOff);
	}
	start += 6 * floatSize;
	int dirLightCount = m_DirectionalLights.size();
	int pointLightCount = m_PointLights.size();
	int spotLightCount = m_SpotLights.size();
	glBufferSubData(GL_UNIFORM_BUFFER, start, sizeof(int), &dirLightCount);
	glBufferSubData(GL_UNIFORM_BUFFER, start + intSize, sizeof(int), &pointLightCount);
	glBufferSubData(GL_UNIFORM_BUFFER, start + 2 * intSize, sizeof(int), &spotLightCount);

	start += 4 * intSize;
	for (unsigned int i = 0; i < m_PointLights.size() && i < 8; ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * floatSize + start, 4, &m_PointLights[i]->Radius);
	}
}

void Scene::UpdateDefaultMaterial()
{
	//	glm::vec3 diffuseColor = glm::vec3(0.6f);
	//	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
	//	glm::vec3 specularColor = glm::vec3(1.0f);
	//	for (int i = 0; i < m_DirectionalLights.size(); i++)
	//	{
	//		defaultMaterial->SetVector(string("dirLights[" + i) + "].diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	//		defaultMaterial->SetVector(string("dirLights[" + i) + "].specular", specularColor);
	//	}
	//	for (int i = 0; i < m_PointLights.size(); i++)
	//	{
	//		defaultMaterial->SetVector(string("pointLights[" + i) + "].diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	//		defaultMaterial->SetVector(string("pointLights[" + i) + "].specular", specularColor);
	//	}
	//	for (int i = 0; i < m_SpotLights.size(); i++)
	//	{
	//		defaultMaterial->SetVector(string("spotLights[" + i) + "].diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	//		defaultMaterial->SetVector(string("spotLights[" + i) + "].specular", specularColor);
	//	}
}

void Scene::render(SceneNode* node)
{
	if (node != nullptr)
	{
		for (int i = 0; i < node->GetChildCount(); i++)
		{
			if (node->GetChildByIndex(i) != nullptr)
			{
				node->GetChildByIndex(i)->Render();
			}
			render(node->GetChildByIndex(i));
		}
	}
}