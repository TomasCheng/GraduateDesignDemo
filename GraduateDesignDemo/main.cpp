#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << "key:" << key << std::endl;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		//����UP�����ӻ�ϱ���
	}
	//		key_UD = key_UD + 0.1f;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		//����DOWN��С��ϱ���
	}
	//		key_UD = key_UD - 0.1f;
}

int main()
{
	std::cout << "hello,world" << endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	//��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//����ǰ���ڰ󶨵�glfw������
	glfwMakeContextCurrent(window);

	//��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//�󶨵����ڴ�С�����仯ʱ�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//�󶨰����ص�
	glfwSetKeyCallback(window, key_callback);

	//��������ͼƬ
	int width, height, nrChannels;
	int width2, height2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

	unsigned char* data2 = stbi_load("container.jpg", &width2, &height2, &nrChannels2, 0);


	//��������
	GLuint texture[2];
	glGenTextures(2, texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);

	//	//�ü��غõ�ͼƬ���ݴ�������
	//	if (data && data2)
	//	{
	//		glGenerateMipmap(GL_TEXTURE_2D);
	//	}
	//	else
	//	{
	//		std::cout << "Failed to load texture" << std::endl;
	//	}
	//���ɺ��ˣ��ͷŸոմ��ļ��м��س�����ͼ������
	stbi_image_free(data);
	stbi_image_free(data2);

	//׼������
	float vertices[][32] = {
		{
			//λ��                  ��ɫ               ��������
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
		},
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.2f, 0.5f, 0.0f
		},
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			0.2f, 0.5f, 0.0f
		}
	};
	//������������
	unsigned int indeces[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);


	GLuint VAO[3];
	glGenVertexArrays(3, VAO);


	GLuint VBO[3];
	//�������㻺�����
	glGenBuffers(3, VBO);
	//�󶨶��㻺����󣬺������й��ڻ������Ĳ�������������vboָ���Ķ������������
	for (int i = 0; i < 3; ++i)
	{
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


		//������
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
		//�򿪶���λ�����ԣ�0��λ��
		glEnableVertexAttribArray(0);
		//ָ�����㻺�������ݸ�ʽ
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		//�򿪶�����ɫ���ԣ�1����ɫ
		glEnableVertexAttribArray(1);
		//ָ�����㻺�������ݸ�ʽ
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		//�򿪶��������������ԣ�2����������
		glEnableVertexAttribArray(2);
		//ָ�����㻺�������ݸ�ʽ
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}


	//
	//	// 0. ���ƶ������鵽�����й�OpenGLʹ��
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//	// 1. ���ö�������ָ��
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);


	//����Shader
	Shader* shader = new Shader("Shader.vs", "Shader.fs");

	shader->use();

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture[0]);
	shader->setInt("ourTexture1", 0);


//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, texture[1]);



	shader->setInt("ourTexture2", 1);


	//��ѭ�����жϴ����Ƿ�Ҫ�ر�
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//		glDrawArrays(GL_LINES, 0, 4);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//�����ύ

		int time = glfwGetTime();

		//		cout << time%10 /10.0f << endl;

		//		if (time % 6 < 3)
		//		{
		//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//		}
		//		else
		//		{
		//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//		}


		//		shader->setFloat("move", time % 10 / 10.0f);

		//		glBindVertexArray(VAO[time % 3]);
		glBindVertexArray(VAO[0]);


		//api��Ϥ�׶�


		//����ǰ��̨֡����
		glfwSwapBuffers(window);
		//�׳��¼�
		glfwPollEvents();
	}
	//glfwֹͣ
	glfwTerminate();

	std::cout << "close the window" << endl;
	return 0;
}
