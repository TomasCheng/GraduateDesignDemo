#include "Camera.h"
#include "Shader.h"
#include "stb_image.h"
#include "Grass.h"

GLfloat grassVertices[] = {
	-0.5f,0.0f,0.0f,  0.0f,0.0f,-1.0f,  1.0f,1.0f,
	0.5f,0.0f,0.0f,  0.0f,0.0f,-1.0f,  0.0f,1.0f,
	0.5f,1.0f,0.0f,  0.0f,0.0f,-1.0f,  0.0f,0.0f,
	-0.5f,1.0f,0.0f,  0.0f,0.0f,-1.0f,  1.0f,0.0f,
};

GLuint grassindeies[] = {
	0,1,2,
	2,3,0
};
GLuint grassVAO;
GLuint grassVBO;
GLuint grassEBO;

GLuint grassDiffuseMap;

bool isgrassInit = false;

GLuint loadTexture2(std::string fileName, GLint REPEAT, GLint FILTER)
{
	//��������
	GLuint texture;
	glGenTextures(1, &texture);
	//������
	glBindTexture(GL_TEXTURE_2D, texture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FILTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FILTER);

	// ��������
	int width, height, chanels;
	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &chanels, 0);
	// ��������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//�ͷ�ͼ����ڴ沢����������
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, texture);

	return texture;
}

void grassInit()
{
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glGenBuffers(1, &grassEBO);

	glBindVertexArray(grassVAO);

	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 32, grassVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 6));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grassEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, grassindeies, GL_STATIC_DRAW);
	glBindVertexArray(0);

	grassDiffuseMap = loadTexture2("grass.png", GL_CLAMP_TO_EDGE, GL_LINEAR);
}

void Drawgrass(Shader *shader, Camera mainCamera)
{
	if (!isgrassInit)
	{
		grassInit();
		isgrassInit = true;
	}
	glBindVertexArray(grassVAO);
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassDiffuseMap);
	shader->setInt("texture_diffuse1", 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}