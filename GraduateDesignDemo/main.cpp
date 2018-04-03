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


	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//将当前窗口绑定到glfw上下文
	glfwMakeContextCurrent(window);

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//绑定当窗口大小发生变化时的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//准备数据
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	float vertices2[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,
	};
	unsigned int VAO1[2];
	glGenVertexArrays(2, VAO1);

	glBindVertexArray(VAO1[0]);


	GLuint VBO;
	//产生顶点缓冲对象
	glGenBuffers(1, &VBO);
	//绑定顶点缓冲对象，后面所有关于缓冲对象的操作都是作用在vbo指定的顶点数组对象上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//传数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//打开顶点属性，0是位置
	glEnableVertexAttribArray(0);
	//指定顶点缓冲中数据格式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(VAO1[1]);


	GLuint VBO2;
	//产生顶点缓冲对象
	glGenBuffers(1, &VBO2);
	//绑定顶点缓冲对象，后面所有关于缓冲对象的操作都是作用在vbo指定的顶点数组对象上
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//传数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	//打开顶点属性，0是位置
	glEnableVertexAttribArray(0);
	//指定顶点缓冲中数据格式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//
	//	// 0. 复制顶点数组到缓冲中供OpenGL使用
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//	// 1. 设置顶点属性指针
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO1[0]);

	//创建Shader
	Shader* shader = new Shader("Shader.vs", "Shader.fs");
	shader->use();


	//主循环，判断窗口是否要关闭
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_LINES, 0, 4);

		//		float timeValue = glfwGetTime();
		//		cout << timeValue << endl;
		//		float move = ((sin(timeValue/100.0f)) +1.0f)/2.0f;
		//		cout << move << endl;
		//		shader.setFloat("move", move);

		//测试提交
		

		int time = glfwGetTime() ;

		cout << time << endl;

		if(time %2 == 0)
		{
			glBindVertexArray(VAO1[0]);

		}else
		{
			glBindVertexArray(VAO1[1]);

		}


		//api熟悉阶段


		//交换前后台帧缓存
		glfwSwapBuffers(window);
		//抛出事件
		glfwPollEvents();
	}
	//glfw停止
	glfwTerminate();

	cout << "close the window" << endl;
	return 0;
}
