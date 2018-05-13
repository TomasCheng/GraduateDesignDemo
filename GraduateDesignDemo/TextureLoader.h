#pragma once

#include "glad/glad.h"

#include <string>

class Texture;
class TextureCube;

/*

Manages all custom logic for loading a variety of different texture files.

*/
class TextureLoader
{
public:
	static Texture LoadTexture(std::string path, GLenum target, GLenum internalFormat, bool srgb = false);
	static Texture LoadHDRTexture(std::string path);
	static TextureCube LoadTextureCube(std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back);
	// assumes default names for cubemap faces
	static TextureCube LoadTextureCube(std::string folder);
};
