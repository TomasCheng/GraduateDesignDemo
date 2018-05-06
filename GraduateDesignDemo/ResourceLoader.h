#pragma once

#include <map>
#include <string>
#include "Shader.h"
#include "MD5.h"

class SceneNode;

/*

Global resource manager. Manages and maintains all resource memory used throughout the
rendering application. New resources are loaded from here, and duplicate resource loads
are prevented. Every resource is referenced by a hashed string ID.

*/
class ResourceLoader
{
private:
	// we index all resources w/ a hashed string ID
	static std::map<unsigned int, Shader>      m_Shaders;
	static std::map<unsigned int, Texture>     m_Textures;
	static std::map<unsigned int, TextureCube> m_TexturesCube;
	static std::map<string, SceneNode*>  m_Meshes;
public:

private:
	// disallow creation of any Resources object; it's defined as a static object
	ResourceLoader();
public:
	static void Init();
	static void Clean();

	// shader resources
	static Shader*      LoadShader(std::string name, std::string vsPath = "DefaultVertShader.vert", std::string fsPath = "DefaultFragShader.frag", std::vector<std::string> defines = std::vector<std::string>());
	static Shader*      GetShader(std::string name);
	// texture resources
	static Texture*     LoadTexture(std::string name, std::string path, GLenum target = GL_TEXTURE_2D, GLenum format = GL_RGBA, bool srgb = false);
	static Texture*     LoadHDR(std::string name, std::string path);
	static TextureCube* LoadTextureCube(std::string name, std::string folder);
	static Texture*     GetTexture(std::string name);
	static TextureCube* GetTextureCube(std::string name);
	// mesh/scene resources
	static SceneNode*  LoadMesh(std::string name, std::string path);
	static SceneNode*  GetMesh(std::string name);

	static MD5* md5;
};

#define SID(string) custom_simple_hash(string)

inline unsigned int custom_simple_hash(const std::string &str)
{
	unsigned int hash = 0;

	for (auto& it : str) {
		// NOTE(Joey): be sure to use prime numbers
		hash = 37 * hash + 17 * static_cast<char>(it);
	}

	// NOTE(Joey): hash value will automatically wrap around
	return hash;
}
