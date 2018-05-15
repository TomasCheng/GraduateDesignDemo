#pragma once

#include <map>
#include <string>
#include "Shader.h"
#include "MD5.h"

class SceneNode;

class ResourceLoader
{
private:
	static std::map<unsigned int, Shader>      m_Shaders;
	static std::map<unsigned int, Texture>     m_Textures;
	static std::map<unsigned int, TextureCube> m_TexturesCube;
	static std::map<string, SceneNode*>  m_Meshes;
public:

private:
	ResourceLoader();
public:
	static void Init();
	static void Clean();

	static Shader*      LoadShader(std::string name, std::string vsPath = "DefaultVertShader.vert", std::string fsPath = "DefaultFragShader.frag", std::vector<std::string> defines = std::vector<std::string>());
	static Shader*      GetShader(std::string name);
	static Texture*     LoadTexture(std::string name, std::string path, GLenum target = GL_TEXTURE_2D, GLenum format = GL_RGBA, bool srgb = false);
	static Texture*     LoadHDR(std::string name, std::string path);
	static TextureCube* LoadTextureCube(std::string name, std::string folder);
	static Texture*     GetTexture(std::string name);
	static TextureCube* GetTextureCube(std::string name);
	static SceneNode*  LoadMesh(std::string name, std::string path);
	static SceneNode*  GetMesh(std::string name);

	static MD5* md5;
};

#define SID(string) custom_simple_hash(string)

inline unsigned int custom_simple_hash(const std::string &str)
{
	unsigned int hash = 0;

	for (auto& it : str) {
		hash = 37 * hash + 17 * static_cast<char>(it);
	}

	return hash;
}
