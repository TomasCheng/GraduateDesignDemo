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

	//加载纹理图片
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	//生成纹理
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//用加载好的图片数据创建纹理
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//生成好了，释放刚刚从文件中加载出来的图像数据
	stbi_image_free(data);

	//准备数据
	float vertices[][32] = {
		{
			//位置              颜色               纹理坐标
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
	//定义索引数据
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
	//产生顶点缓冲对象
	glGenBuffers(3, VBO);
	//绑定顶点缓冲对象，后面所有关于缓冲对象的操作都是作用在vbo指定的顶点数组对象上
	for (int i = 0; i < 3; ++i)
	{
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


		//传数据
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
		//打开顶点位置属性，0是位置
		glEnableVertexAttribArray(0);
		//指定顶点缓冲中数据格式
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		//打开顶点颜色属性，1是颜色
		glEnableVertexAttribArray(1);
		//指定顶点缓冲中数据格式
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		//打开顶点纹理坐标属性，2是纹理坐标
		glEnableVertexAttribArray(2);
		//指定顶点缓冲中数据格式
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}


	//
	//	// 0. 复制顶点数组到缓冲中供OpenGL使用
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//	// 1. 设置顶点属性指针
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);


	//创建Shader
	Shader* shader = new Shader("Shader.vs", "Shader.fs");
	shader->use();


	//主循环，判断窗口是否要关闭
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//		glDrawArrays(GL_LINES, 0, 4);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//测试提交

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
