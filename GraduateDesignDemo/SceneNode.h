#pragma once
#include "Material.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glad/glad.h"
#include "MeshLoader.h"
#include "Scene.h"

class SceneNode
{
public:

	void Render();

	// each node contains relevant render state
	Mesh * mesh;
	Material* material;

	// bounding box
	glm::vec3 BoxMin = glm::vec3(-99999.0f);
	glm::vec3 BoxMax = glm::vec3(99999.0f);
private:
	std::vector<SceneNode*> m_Children = std::vector<SceneNode*>();
	SceneNode *m_Parent;

	glm::mat4 m_Transform = glm::mat4(1.0);
	glm::mat4 m_PrevTransform = glm::mat4(1.0);
	glm::vec3 m_Position = glm::vec3(0.0f);
	glm::vec4 m_Rotation = glm::vec4(0, 1, 0, 0); // axis-angle for now; test w/ quaternions soon!
	glm::vec3 m_Scale = glm::vec3(1.0f);

	// mark the current node's tranform as dirty if it needs to be re-calculated this frame
	bool m_Dirty;

	// each node is uniquely identified by a 32-bit incrementing unsigned integer
	unsigned int m_ID;

	static unsigned int CounterID;
public:
	SceneNode(SceneNode* parent = Scene::Root);
	SceneNode(Mesh* mesh, Material* mat, SceneNode* parent = Scene::Root);
	~SceneNode();

	// scene node transform
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec4 rotation);
	void SetScale(glm::vec3 scale);
	void SetScale(float scale);
	glm::vec3 GetLocalPosition();
	glm::vec4 GetLocalRotation();
	glm::vec3 GetLocalScale();
	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldScale();

	// scene graph
	unsigned int GetID();
	void AddChild(SceneNode *node);
	void RemoveChild(unsigned int id);
	std::vector<SceneNode*> GetChildren();
	unsigned int            GetChildCount();
	SceneNode              *GetChild(unsigned int id);
	SceneNode              *GetChildByIndex(unsigned int index);
	SceneNode              *GetParent();

	// returns the transform of the current node combined with its parent(s)' transform.
	glm::mat4 GetTransform();
	glm::mat4 GetPrevTransform();

	// re-calculates this node and its children's transform components if its parent or the
	// node itself is dirty.
	void UpdateTransform(bool updatePrevTransform = false);

	string GetName();
};
