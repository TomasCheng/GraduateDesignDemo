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
	//����Shader�е�����
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

	//��ͼ
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

	//�ָ�״̬
	glActiveTexture(GL_TEXTURE0);
	//�ָ�״̬
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

	//�Զ������ݸ�ʽ����˵��
	//λ��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(0));
	//����
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, Normal)));
	//��������
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, TexCoords)));

	//�ָ�״̬��ÿ�λ�����֮ǰ�����õ�ǰ����״̬������֮���ٻָ�
	glBindVertexArray(0);
}