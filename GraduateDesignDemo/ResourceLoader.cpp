#include "ResourceLoader.h"

#include <stack>
#include <vector>
#include "Logger.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "MeshLoader.h"
#include "Scene.h"
#include "SceneNode.h"

std::map<unsigned int, Shader>      ResourceLoader::m_Shaders = std::map<unsigned int, Shader>();
std::map<unsigned int, Texture>     ResourceLoader::m_Textures = std::map<unsigned int, Texture>();
std::map<unsigned int, TextureCube> ResourceLoader::m_TexturesCube = std::map<unsigned int, TextureCube>();
std::map<string, SceneNode*>  ResourceLoader::m_Meshes = std::map<string, SceneNode*>();
MD5* ResourceLoader::md5 = new MD5;
// --------------------------------------------------------------------------------------------
void ResourceLoader::Init()
{
	// initialize default assets/ResourceLoader that should  always be available, regardless of
	// configuration.
	Texture placeholderTexture;
}
void ResourceLoader::Clean()
{
	// traverse all stored mesh scene nodes and delete accordingly.
	// Note that this time we don't care about deleting dangling pointers as each scene node is
	// unique and shouldn't reference other scene nodes than their children.
	for (auto it = m_Meshes.begin(); it != m_Meshes.end(); it++)
	{
		delete it->second;
	}
}

// --------------------------------------------------------------------------------------------
Shader* ResourceLoader::LoadShader(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines)
{
	unsigned int id = SID(name);

	// if shader already exists, return that handle
	if (ResourceLoader::m_Shaders.find(id) != ResourceLoader::m_Shaders.end())
		return &ResourceLoader::m_Shaders[id];

	Shader shader = ShaderLoader::Load(name, vsPath, fsPath, defines);
	ResourceLoader::m_Shaders[id] = shader;
	return &ResourceLoader::m_Shaders[id];
	//	return &shader;
}
// --------------------------------------------------------------------------------------------
Shader* ResourceLoader::GetShader(std::string name)
{
	unsigned int id = SID(name);

	// if shader exists, return that handle
	if (ResourceLoader::m_Shaders.find(id) != ResourceLoader::m_Shaders.end())
	{
		return &ResourceLoader::m_Shaders[id];
	}
	else
	{
		Log::Message("Requested shader: " + name + " not found!", LOG_WARNING);
		return nullptr;
	}
}
// --------------------------------------------------------------------------------------------
Texture* ResourceLoader::LoadTexture(std::string name, std::string path, GLenum target, GLenum format, bool srgb)
{
	unsigned int id = SID(name);

	// if texture already exists, return that handle
	if (ResourceLoader::m_Textures.find(id) != ResourceLoader::m_Textures.end())
		return &ResourceLoader::m_Textures[id];

	Log::Message("Loading texture file at: " + path + ".", LOG_INIT);

	Texture texture = TextureLoader::LoadTexture(path, target, format, srgb);

	Log::Message("Succesfully loaded: " + path + ".", LOG_INIT);

	// make sure texture got properly loaded
	if (texture.Width > 0)
	{
		ResourceLoader::m_Textures[id] = texture;
		return &ResourceLoader::m_Textures[id];
	}
	else
	{
		return nullptr;
	}
}
// --------------------------------------------------------------------------------------------
Texture* ResourceLoader::LoadHDR(std::string name, std::string path)
{
	unsigned int id = SID(name);

	// if texture already exists, return that handle
	if (ResourceLoader::m_Textures.find(id) != ResourceLoader::m_Textures.end())
		return &ResourceLoader::m_Textures[id];

	Texture texture = TextureLoader::LoadHDRTexture(path);
	// make sure texture got properly loaded
	if (texture.Width > 0)
	{
		ResourceLoader::m_Textures[id] = texture;
		return &ResourceLoader::m_Textures[id];
	}
	else
	{
		return nullptr;
	}
}
// --------------------------------------------------------------------------------------------
Texture* ResourceLoader::GetTexture(std::string name)
{
	unsigned int id = SID(name);

	// if shader exists, return that handle
	if (ResourceLoader::m_Textures.find(id) != ResourceLoader::m_Textures.end())
	{
		return &ResourceLoader::m_Textures[id];
	}
	else
	{
		Log::Message("Requested texture: " + name + " not found!", LOG_WARNING);
		return nullptr;
	}
}
// --------------------------------------------------------------------------------------------
TextureCube* ResourceLoader::LoadTextureCube(std::string name, std::string folder)
{
	unsigned int id = SID(name);

	// if texture already exists, return that handle
	if (ResourceLoader::m_TexturesCube.find(id) != ResourceLoader::m_TexturesCube.end())
		return &ResourceLoader::m_TexturesCube[id];

	TextureCube texture = TextureLoader::LoadTextureCube(folder);
	ResourceLoader::m_TexturesCube[id] = texture;
	return &ResourceLoader::m_TexturesCube[id];
}
// --------------------------------------------------------------------------------------------
TextureCube* ResourceLoader::GetTextureCube(std::string name)
{
	unsigned int id = SID(name);

	// if shader exists, return that handle
	if (ResourceLoader::m_TexturesCube.find(id) != ResourceLoader::m_TexturesCube.end())
	{
		return &ResourceLoader::m_TexturesCube[id];
	}
	else
	{
		Log::Message("Requested texture cube: " + name + " not found!", LOG_WARNING);
		return nullptr;
	}
}
// --------------------------------------------------------------------------------------------
SceneNode* ResourceLoader::LoadMesh(std::string name, std::string path)
{
	//	unsigned int id = SID(name + path);

	md5->update(name);
	string id = md5->toString();
	md5->reset();
	// if mesh's scene node was already loaded before, copy the scene node's memory and return
	// the copied reference. We return a copy as the moment the global scene deletes the
	// returned node, all other and next requested scene nodes of this model will end up as
	// dangling pointers.
	if (ResourceLoader::m_Meshes.find(id) != ResourceLoader::m_Meshes.end())
	{
		return (ResourceLoader::m_Meshes[id]);
	}

	// MeshLoader::LoadMesh initializes a scene node hierarchy on the heap. We are responsible
	// for managing the memory; keep a reference to the root node of the model scene.
	SceneNode* node = MeshLoader::LoadMesh(path);
	ResourceLoader::m_Meshes[id] = node;

	// return a copied reference through the scene to prevent dangling pointers.
	// See description above.
	return (node);
}
// --------------------------------------------------------------------------------------------
SceneNode* ResourceLoader::GetMesh(std::string name)
{
	//	unsigned int id = SID(name);
	md5->update(name);
	string id = md5->toString();
	md5->reset();
	// if mesh's scene node was already loaded before, copy the scene node's memory and return
	// the copied reference. We return a copy as the moment the global scene deletes the
	// returned node, all other and next requested scene nodes of this model will end up as
	// dangling pointers.
	if (ResourceLoader::m_Meshes.find(id) != ResourceLoader::m_Meshes.end())
	{
		return (ResourceLoader::m_Meshes[id]);
	}
	else
	{
		Log::Message("Requested mesh: " + name + " not found!", LOG_WARNING);
		return nullptr;
	}
}