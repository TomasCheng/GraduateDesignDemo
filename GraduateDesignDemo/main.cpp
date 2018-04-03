#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	cout << "hello,world" << endl;
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

	//׼������
	float vertices[][12] = {
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
			0.0f, 0.0f, 0.0f
		},
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
			-0.5f, 0.0f, 0.0f,
		},
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
			0.5f, 0.0f, 0.0f,
		}
	};
	GLuint VAO[3];
	glGenVertexArrays(3, VAO);


	int m = sizeof(vertices[0]);

	GLuint VBO[3];
	//�������㻺�����
	glGenBuffers(3, VBO);
	//�󶨶��㻺����󣬺������й��ڻ������Ĳ�������������vboָ���Ķ������������
	for (int i = 0; i < 3; ++i)
	{
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

		

		//������
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_DYNAMIC_DRAW);
		//�򿪶������ԣ�0��λ��
		glEnableVertexAttribArray(0);
		//ָ�����㻺�������ݸ�ʽ
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
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


	//��ѭ�����жϴ����Ƿ�Ҫ�ر�
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_LINES, 0, 4);

		//�����ύ

		int time = glfwGetTime();

//		cout << time%10 /10.0f << endl;


//		shader->setFloat("move", time % 10 / 10.0f);

		glBindVertexArray(VAO[time % 3]);


		//api��Ϥ�׶�


		//����ǰ��̨֡����
		glfwSwapBuffers(window);
		//�׳��¼�
		glfwPollEvents();
	}
	//glfwֹͣ
	glfwTerminate();

	cout << "close the window" << endl;
	return 0;
}
