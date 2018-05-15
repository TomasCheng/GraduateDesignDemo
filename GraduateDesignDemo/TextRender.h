#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "glad/glad.h"
#include <glm/vec2.hpp>
#include <map>
#include "Shader.h"

struct Character {
	GLuint     TextureID;  // 字形纹理的ID
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	GLuint     Advance;    // 原点距下一个字形原点的距离
};

class TextRender
{
public:
	TextRender();
	~TextRender();

	static void Init(int width, int height);

	static void Render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
	static FT_Library ft;
	static FT_Face face;
	static std::map<GLchar, Character> Characters;

	static Shader* shader;
	static GLuint VAO, VBO;
};
