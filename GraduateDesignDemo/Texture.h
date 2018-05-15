#pragma once
#pragma once

#include "glad/glad.h"

class Texture
{
public:
	unsigned int ID;
	GLenum Target = GL_TEXTURE_2D;
	GLenum InternalFormat = GL_RGBA;
	GLenum Format = GL_RGBA;
	GLenum Type = GL_UNSIGNED_BYTE;
	GLenum FilterMin = GL_LINEAR;
	GLenum FilterMax = GL_LINEAR;
	GLenum WrapS = GL_REPEAT;
	GLenum WrapT = GL_REPEAT;
	GLenum WrapR = GL_REPEAT;
	bool Mipmapping = true;

	unsigned int Width = 0;
	unsigned int Height = 0;
	unsigned int Depth = 0;
private:
public:
	Texture();
	~Texture();

	void Generate(unsigned int width, GLenum internalFormat, GLenum format, GLenum type, void* data);
	void Generate(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, void* data);
	void Generate(unsigned int width, unsigned int height, unsigned int depth, GLenum internalFormat, GLenum format, GLenum type, void* data);

	void Resize(unsigned int width, unsigned int height = 0, unsigned int depth = 0);

	void Bind(int unit = -1);
	void Unbind();

	void SetWrapMode(GLenum wrapMode, bool bind = false);
	void SetFilterMin(GLenum filter, bool bind = false);
	void SetFilterMax(GLenum filter, bool bind = false);
};
