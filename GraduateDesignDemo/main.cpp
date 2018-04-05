#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/fwd.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float x;
Shader* shader;
glm::mat4 trans(1.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//	std::cout << "key:" << key << std::endl;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		//		x += 0.01f;
		//		shader->setFloat("x", x);
		//		std::cout << "x:" << x << std::endl;
		//		//按下UP键增加混合比例
		//		x += 1.0f;
		trans = glm::rotate(trans, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		shader->setMat4("transform", trans);
	}
	//		key_UD = key_UD + 0.1f;
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
	{
		//		x -= 0.01f;
		//		shader->setFloat("x", x);
		//		std::cout << "x:" << x << std::endl;

		trans = glm::rotate(trans, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shader->setMat4("transform", trans);

		//按下DOWN减小混合比例
	}
	//		key_UD = key_UD - 0.1f;
}

int main()
{
	std::cout << "hello,world" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	//test
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//	trans = glm::rotate(trans, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	vec = trans * vec;

	std::cout << vec.x << std::endl << vec.y << std::endl << vec.z << std::endl << vec.w << std::endl;


	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, 800, 800);

	//绑定当窗口大小发生变化时的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//绑定按键回调
	glfwSetKeyCallback(window, key_callback);

	//加载纹理图片
	int width, height, nrChannels;
	int width2, height2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

	unsigned char* data2 = stbi_load("container.jpg", &width2, &height2, &nrChannels2, 0);


	//生成纹理
	GLuint texture[2];
	glGenTextures(2, texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);

	//	//用加载好的图片数据创建纹理
	//	if (data && data2)
	//	{
	//		glGenerateMipmap(GL_TEXTURE_2D);
	//	}
	//	else
	//	{
	//		std::cout << "Failed to load texture" << std::endl;
	//	}
	//生成好了，释放刚刚从文件中加载出来的图像数据
	stbi_image_free(data);
	stbi_image_free(data2);

	//准备数据
	float vertices[][32] = {
		{
			//位置                  颜色               纹理坐标
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
	//正方体数据,全顶点，非索引
	GLfloat cubeVertices[] = {
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	GLuint cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	GLuint cubeVBO;
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//定义索引数据
	unsigned int indeces[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
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
	shader = new Shader("Shader.vs", "Shader.fs");

	shader->use();

	//	shader->setInt("ourTexture1", 0);

	shader->setInt("ourTexture2", 1);


	//MVP矩阵的构造
//	glm::mat4 modle(1.0f);
//	modle = glm::rotate(modle, glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
//
//	trans = proj * view * modle;

	shader->setMat4("view", view);
	shader->setMat4("proj", proj);

	glEnable(GL_DEPTH_TEST);

	//主循环，判断窗口是否要关闭
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int time = glfwGetTime();

		for (int i = 0; i < 10; ++i)
		{
			glm::mat4 modle(1.0f);
			modle = glm::translate(modle, cubePositions[i]);
			modle = glm::rotate(modle, glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//			glm::mat4 view(1.0f);
////			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//			view = glm::ortho(-20.0f,20.0f,-20.0f,20.0f,0.01f,100.0f);
//			glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
//
//			trans = proj * view * modle;
//
//			float angle = 20.0f * i;
			x += 0.001f;
			modle = glm::rotate(modle, x/10.0f * (i+1), glm::vec3(sin(x*i), cos(x), 0.0f));


			shader->setMat4("model", modle);

 
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}


		//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//测试提交


		//		cout << time%10 /10.0f << endl;
				std::cout << time <<std:: endl;

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
		//		glBindVertexArray(VAO[0]);
		glBindVertexArray(cubeVAO);



		//api熟悉阶段


		//交换前后台帧缓存
		glfwSwapBuffers(window);
		//抛出事件
		glfwPollEvents();
	}
	//glfw停止
	glfwTerminate();

	std::cout << "close the window" << std::endl;
	return 0;
}
