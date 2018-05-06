#include "TextRender.h"
#include <ostream>
#include <iostream>
#include "ResourceLoader.h"

FT_Library TextRender::ft = nullptr;
FT_Face TextRender::face = nullptr;
std::map<GLchar, Character> TextRender::Characters = std::map<GLchar, Character>();

Shader* TextRender::shader = nullptr;
GLuint TextRender::VAO = 0;
GLuint TextRender::VBO = 0;

TextRender::TextRender()
{
}

TextRender::~TextRender()
{
}

void TextRender::Init(int width, int height)
{
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	if (FT_New_Face(ft, "extra_font/Karla-Regular.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�����ֽڶ�������
	for (GLubyte c = 0; c < 128; c++)
	{
		// �����ַ�������
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// ��������
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// ��������ѡ��
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// �����ַ���֮��ʹ��
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader = ResourceLoader::LoadShader("textShader", "TextShader.vert", "TextShader.frag");
	shader->Use();
	glm::mat4 projection = glm::ortho(0.0f, width *1.0f, 0.0f, height*1.0f);
	shader->SetMatrix("projection", projection);
}

void TextRender::Render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// �����Ӧ����Ⱦ״̬
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->Use();
	shader->SetVector("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// �����ı������е��ַ�
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// ��ÿ���ַ�����VBO
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// ���ı����ϻ�����������
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// ����VBO�ڴ������
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// �����ı���
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
		x += (ch.Advance >> 6) * scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}