#include "Mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->setupMesh();
}

void Mesh::Draw(Shader shader)
{
	//设置Shader中的纹理
	unsigned int diffuseNum = 0;
	unsigned int specularNum = 0;
	for (unsigned int i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		const string name = textures[i].type;
		if (name == "texture_diffuse")
		{
			number = to_string(diffuseNum++);
		}
		else if (name == "texture_specular")
		{
			number = to_string(specularNum++);
		}
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		shader.setInt(("material." + name + number).c_str(), i);
	}

	//画图
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

	//恢复状态
	glActiveTexture(GL_TEXTURE0);
	//恢复状态
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*this->indices.size(), &this->indices[0], GL_STATIC_DRAW);

	//对顶点数据格式进行说明
	//位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(0));
	//法线
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, Normal)));
	//纹理坐标
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, TexCoords)));

	//恢复状态，每次画东西之前再设置当前所需状态，结束之后再恢复
	glBindVertexArray(0);
}