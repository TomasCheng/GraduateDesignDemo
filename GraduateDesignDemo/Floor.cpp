#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Floor.h"

GLfloat floorVeterx[] = {
	-1.0f,0.0f,-1.0f,  0.0f,1.0f,0.0f,  0.0f,0.0f,
	1.0f,0.0f,-1.0f,  0.0f,1.0f,0.0f,  10.0f,0.0f,
	1.0f,0.0f,1.0f,  0.0f,1.0f,0.0f,  10.0f,10.0f,
	-1.0f,0.0f,1.0f,  0.0f,1.0f,0.0f,  0.0f,10.0f,
};
GLuint floorindeies[] = {
	0,1,2,
	2,3,0
};
GLuint floorVAO;
GLuint floorVBO;
GLuint floorEBO;

GLuint floorDiffuseMap;

bool isFloorInit = false;

void InitFloorDiffuse()
{
	glGenTextures(1, &floorDiffuseMap);
	glBindTexture(GL_TEXTURE_2D, floorDiffuseMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLubyte Image[4][4][4];
	int i, j, c;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			c = (((i & 0x2) == 0) ^ ((j & 0x2) == 0)) * 255;
			//¼ÆËãÎÆÀíÑÕÉ«
			Image[i][j][0] = GLubyte(c);
			Image[i][j][1] = GLubyte(c);
			Image[i][j][2] = GLubyte(c);
			Image[i][j][3] = GLubyte(255);
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image);
}

void FloorInit()
{
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glGenBuffers(1, &floorEBO);

	glBindVertexArray(floorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 32, floorVeterx, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 6));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, floorindeies, GL_STATIC_DRAW);
	glBindVertexArray(0);

	floorDiffuseMap = loadTexture("timg3.jpg", GL_REPEAT, GL_LINEAR);
	//	InitFloorDiffuse();
}

void DrawFloor(Shader *shader)
{
	if (!isFloorInit)
	{
		FloorInit();
		isFloorInit = true;
	}
	glBindVertexArray(floorVAO);
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorDiffuseMap);
	shader->setInt("material.texture_diffuse0", 0);
	shader->setInt("material.texture_specular0", 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}