#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "glad/glad.h"
#include <glm/vec2.hpp>
#include <map>
#include "Shader.h"

struct Character {
	GLuint     TextureID;  // ���������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
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
