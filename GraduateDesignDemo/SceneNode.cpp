#include "SceneNode.h"

#include <assert.h>

unsigned int SceneNode::CounterID = 0;

void SceneNode::Render()
{
	this->UpdateTransform();

	if (this->mesh == nullptr)
	{
		return;
	}
	this->material->Use();
	this->material->SetMatrix("model", m_Transform);

	glBindVertexArray(this->mesh->m_VAO);
	if (this->mesh->Indices.size() > 0)
	{
		glDrawElements(this->mesh->Topology == TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLES, this->mesh->Indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(this->mesh->Topology == TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLES, 0, this->mesh->Positions.size());
	}
}

// --------------------------------------------------------------------------------------------
SceneNode::SceneNode(SceneNode* parent)
{
	m_ID = CounterID++;
	if (parent != nullptr)
	{
		parent->AddChild(this);
	}
}

SceneNode::SceneNode(Mesh* mesh, Material* mat, SceneNode* parent)
{
	this->mesh = mesh;
	this->material = mat;
	m_ID = CounterID++;
	if (parent != nullptr)
	{
		parent->AddChild(this);
	}
}

// --------------------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
	// traverse the list of children and delete accordingly.
	for (unsigned int i = 0; i < m_Children.size(); ++i)
	{
		// it should not be possible that a scene node is childed by more than one
		// parent, thus we don't need to care about deleting dangling pointers.
		delete m_Children[i];
	}
}
// --------------------------------------------------------------------------------------------
void SceneNode::SetPosition(glm::vec3 position)
{
	m_Position = position;
	m_Dirty = true;
}
// --------------------------------------------------------------------------------------------
void SceneNode::SetRotation(glm::vec4 rotation)
{
	m_Rotation = rotation;
	m_Dirty = true;
}
// --------------------------------------------------------------------------------------------
void SceneNode::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
	m_Dirty = true;
}
// --------------------------------------------------------------------------------------------
void SceneNode::SetScale(float scale)
{
	m_Scale = glm::vec3(scale);
	m_Dirty = true;
}
// --------------------------------------------------------------------------------------------
glm::vec3 SceneNode::GetLocalPosition()
{
	return m_Position;
}
// --------------------------------------------------------------------------------------------
glm::vec4 SceneNode::GetLocalRotation()
{
	return m_Rotation;
}
// --------------------------------------------------------------------------------------------
glm::vec3 SceneNode::GetLocalScale()
{
	return m_Scale;
}
// --------------------------------------------------------------------------------------------
glm::vec3 SceneNode::GetWorldPosition()
{
	glm::mat4 transform = GetTransform();
	glm::vec4 pos = transform * glm::vec4(m_Position, 1.0f);
	return glm::vec3(pos);
}
// --------------------------------------------------------------------------------------------
glm::vec3 SceneNode::GetWorldScale()
{
	glm::mat4 transform = GetTransform();
	glm::vec3 scale = glm::vec3(transform[0][0], transform[1][1], transform[2][2]);
	if (scale.x < 0.0f) scale.x *= -1.0f;
	if (scale.y < 0.0f) scale.y *= -1.0f;
	if (scale.z < 0.0f) scale.z *= -1.0f;
	return scale;
}
// --------------------------------------------------------------------------------------------
unsigned int SceneNode::GetID()
{
	return m_ID;
}
// --------------------------------------------------------------------------------------------
void SceneNode::AddChild(SceneNode *node)
{
	// check if this child already has a parent. If so, first remove this scene node from its
	// current parent. Scene nodes aren't allowed to exist under multiple parents.
	if (node->m_Parent)
	{
		node->m_Parent->RemoveChild(node->m_ID);
	}
	node->m_Parent = this;
	m_Children.push_back(node);
}
// --------------------------------------------------------------------------------------------
void SceneNode::RemoveChild(unsigned int id)
{
	auto it = std::find(m_Children.begin(), m_Children.end(), GetChild(id));
	if (it != m_Children.end())
		m_Children.erase(it);
}
// --------------------------------------------------------------------------------------------
std::vector<SceneNode*> SceneNode::GetChildren()
{
	return m_Children;
}
// --------------------------------------------------------------------------------------------
unsigned int SceneNode::GetChildCount()
{
	return m_Children.size();
}
// --------------------------------------------------------------------------------------------
SceneNode *SceneNode::GetChild(unsigned int id)
{
	for (unsigned int i = 0; i < m_Children.size(); ++i)
	{
		if (m_Children[i]->GetID() == id)
			return m_Children[i];
	}
	return nullptr;
}
// --------------------------------------------------------------------------------------------
SceneNode* SceneNode::GetChildByIndex(unsigned int index)
{
	assert(index < GetChildCount());
	return m_Children[index];
}
// --------------------------------------------------------------------------------------------
SceneNode *SceneNode::GetParent()
{
	return m_Parent;
}
// --------------------------------------------------------------------------------------------
glm::mat4 SceneNode::GetTransform()
{
	if (m_Dirty)
	{
		UpdateTransform(false);
	}
	return m_Transform;
}
// --------------------------------------------------------------------------------------------
glm::mat4 SceneNode::GetPrevTransform()
{
	return m_PrevTransform;
}
// --------------------------------------------------------------------------------------------
void SceneNode::UpdateTransform(bool updatePrevTransform)
{
	// if specified, store current transform as prev transform (for calculating motion vectors)
	if (updatePrevTransform)
	{
		m_PrevTransform = m_Transform;
	}
	// we only do this if the node itself or its parent is flagged as dirty
//	if (m_Parent)
//	{
//		m_Dirty = m_Dirty | m_Parent->m_Dirty;
//	}
	if (m_Dirty)
	{
		// first scale, then rotate, then translation
		glm::mat4 m(1.0f);
		m_Transform = glm::translate(m, m_Position);
		m_Transform = glm::scale(m_Transform, m_Scale);
		m_Transform = glm::rotate(m_Transform, m_Rotation.w, glm::vec3(m_Rotation));
		if (m_Parent)
		{
			m_Transform = m_Parent->m_Transform * m_Transform;
		}
	}
	for (int i = 0; i < m_Children.size(); ++i)
	{
		if (m_Dirty)
		{
			m_Children[i]->m_Dirty = true;
		}
		m_Children[i]->UpdateTransform(updatePrevTransform);
	}
	m_Dirty = false;
}

string SceneNode::GetName()
{
	if (mesh == nullptr)
	{
		return "defaultNodeName";
	}
	return mesh->Name;
}