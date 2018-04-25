#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/fwd.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include "Camera.h"
#include "Floor.h"
#include "Grass.h"
#include "CubeMap.h"
using namespace std;

const GLuint WIDTH = 800, HEIGHT = 800;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint loadTexture(string fileName, GLint REPEAT, GLint FILTER);
void do_movement();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void windowSize_callback(GLFWwindow* window, int width, int height);

Camera mainCamera;
Shader modelShader; //modelShader

//灯光
GLuint lightVAO;
glm::vec3 lightPos(0.0f, 2.3f, 0.0f);
glm::vec3 dynamicLightPos = lightPos;
Shader lightShader;
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

GLfloat deltaTime = 0.0f; // 当前帧遇上一帧的时间差
GLfloat lastFrame = 0.0f; // 上一帧的时间

bool keys[1024];

GLfloat lastX = 400, lastY = 300;
GLfloat scrollSpeed = 0.05f;
bool firstMouse = true;

GLuint VAO, VBO;

float moveF;
float moveR;
float moveU;

void lightInit()
{
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(0));
}

void shaderInit()
{
	modelShader = Shader("Shader.vert", "Shader.frag");
	lightShader = Shader("Shader.vert", "Light.frag");
}

GLuint loadTexture(string fileName, GLint REPEAT, GLint FILTER)
{
	//创建纹理
	GLuint texture;
	glGenTextures(1, &texture);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FILTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FILTER);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载纹理
	int width, height, chanels;
	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &chanels, 0);
	// 生成纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//释放图像的内存并解绑纹理对象
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, texture);

	return texture;
}

void vertexObjectInit()
{
	//不使用索引缓冲对象用两个三角形绘制一个梯形
	// 设置顶点缓存和属性指针
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	//创建索引缓冲对象
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	// 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 法向量
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // 这个方法将顶点属性指针注册到VBO作为当前绑定顶点对象，然后我们就可以安全的解绑
	glBindVertexArray(0); // 解绑 VAO (这通常是一个很好的用来解绑任何缓存/数组并防止奇怪错误的方法)
}

int main()
{
	//初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//注册窗口大小发生变化时的回调
	glfwSetWindowSizeCallback(window, windowSize_callback);
	//注册键盘回调
	glfwSetKeyCallback(window, key_callback);
	//注册鼠标回调
	glfwSetCursorPosCallback(window, mouse_callback);
	//注册鼠标滚轮回到
	glfwSetScrollCallback(window, scroll_callback);
	//设置光标隐藏并捕获
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//初始化GlAD

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad" << std::endl;
		return -1;
	}
	//告诉OpenGL渲染窗口尺寸大小
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	//	glDepthMask(GL_FALSE);

		//初始化并绑定shaders
	shaderInit();
	//初始化顶点对象数据
	vertexObjectInit();
	//初始化灯光VAO
	lightInit();

	Model nanoModel("nanosuit/nanosuit.obj");

	lightShader.use();
	lightShader.setVec3("lightColor", lightColor);

	mainCamera = Camera();

	glm::vec3 pointLightPositions[] = {
		glm::vec3(-6.0f, 1.5f, 0.0f),
		glm::vec3(-2.0f, 2.0f, 0.0f),
		glm::vec3(2.0f, 3.0f, 0.0f),
		glm::vec3(6.0f, 4.0f, 0.0f)
	};
	glm::vec3 zeroPos(0.0f);

	//设置shader中的一些不变量
	modelShader.use();
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.6f); //
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); //
	glm::vec3 specularColor = glm::vec3(1.0f);

	modelShader.setVec3("dirLight.ambient", ambientColor);
	modelShader.setVec3("dirLight.diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	modelShader.setVec3("dirLight.specular", specularColor);
	modelShader.setVec3("pointLights[0].ambient", ambientColor);
	modelShader.setVec3("pointLights[0].diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	modelShader.setVec3("pointLights[0].specular", specularColor);
	modelShader.setVec3("pointLights[1].ambient", ambientColor);
	modelShader.setVec3("pointLights[1].diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	modelShader.setVec3("pointLights[1].specular", specularColor);
	modelShader.setVec3("pointLights[2].ambient", ambientColor);
	modelShader.setVec3("pointLights[2].diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	modelShader.setVec3("pointLights[2].specular", specularColor);
	modelShader.setVec3("pointLights[3].ambient", ambientColor);
	modelShader.setVec3("pointLights[3].diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	modelShader.setVec3("pointLights[3].specular", specularColor);
	modelShader.setVec3("spotLight.ambient", ambientColor);
	modelShader.setVec3("spotLight.diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
	modelShader.setVec3("spotLight.specular", specularColor);

	modelShader.setVec3("dirLight.direction", -lightPos);

	modelShader.setVec3("spotLight.position", mainCamera.Position);
	modelShader.setVec3("spotLight.direction", mainCamera.Front);
	modelShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
	modelShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));

	modelShader.setVec3("spotLight[0].position", pointLightPositions[0]);
	modelShader.setFloat("pointLights[0].constant", 1.0f);
	modelShader.setFloat("pointLights[0].linear", 0.09f);
	modelShader.setFloat("pointLights[0].quadratic", 0.032f);
	modelShader.setVec3("spotLight[1].position", pointLightPositions[1]);
	modelShader.setFloat("pointLights[1].constant", 1.0f);
	modelShader.setFloat("pointLights[1].linear", 0.09f);
	modelShader.setFloat("pointLights[1].quadratic", 0.032f);
	modelShader.setVec3("spotLight[2].position", pointLightPositions[2]);
	modelShader.setFloat("pointLights[2].constant", 1.0f);
	modelShader.setFloat("pointLights[2].linear", 0.09f);
	modelShader.setFloat("pointLights[2].quadratic", 0.032f);
	modelShader.setVec3("spotLight[3].position", pointLightPositions[3]);
	modelShader.setFloat("pointLights[3].constant", 1.0f);
	modelShader.setFloat("pointLights[3].linear", 0.09f);
	modelShader.setFloat("pointLights[3].quadratic", 0.032f);

	modelShader.use();
	//材质的设置
	modelShader.setFloat("material.shininess", 32.0f);

	vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	Shader grassShader("Shader.vert", "GrassShader.frag");

	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// 生成纹理
	glActiveTexture(GL_TEXTURE0);
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 将它附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader rttShader = Shader("RTTShader.vert", "RTTShader.frag");

	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式

	//天空盒
	Shader cubeMapShader = Shader("cubeMapShader.vert", "cubeMapShader.frag");

	glDepthFunc(GL_LEQUAL);

	//让窗口接受输入并保持运行
	while (!glfwWindowShouldClose(window))
	{
		for (int count = 0; count < 2; count++)
		{
			if (count == 0)
			{
				//				glViewport(0, 0, width, height);

				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			}
			else
			{
				//				glViewport(0, 0, 10.0f, 10.0f);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			//检查事件
			glfwPollEvents();
			do_movement();

			//渲染指令
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//设置根据时间变换的x，y偏移值，最终效果为圆周运动
			GLfloat timeValue = glfwGetTime();
			GLfloat offsetx = 2 * ((sin(timeValue) / 2) + 0.5);
			GLfloat offsety = 2 * ((cos(timeValue) / 2) + 0.5);
			dynamicLightPos.x = lightPos.x + offsetx;
			dynamicLightPos.z = lightPos.z + offsety;

			glm::mat4 view(1.0f);
			view = mainCamera.GetViewMatrix();

			glm::mat4 proj(1.0f);
			proj = glm::perspective(mainCamera.Zoom * scrollSpeed, (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

			modelShader.use();
			modelShader.setMat4("view", view);
			modelShader.setMat4("proj", proj);

			glBindVertexArray(VAO);

			modelShader.use();
			glm::mat4 model(1.0f);
			model = glm::translate(model, zeroPos);
			model = glm::translate(model, glm::vec3(moveR, moveU, moveF));
			model = glm::scale(model, glm::vec3(0.2f));

			model = glm::rotate(model, glm::radians(timeValue * 20), glm::vec3(0, 1.0f, 0));

			modelShader.setVec3("viewPos", mainCamera.Position);
			modelShader.setVec3("spotLight.position", mainCamera.Position);
			modelShader.setVec3("spotLight.direction", mainCamera.Front);

			modelShader.setMat4("model", model);

			nanoModel.Draw(modelShader);

			grassShader.use();

			//画草
			grassShader.setMat4("view", view);
			grassShader.setMat4("proj", proj);

			for (unsigned int i = 0; i < vegetation.size(); i++)
			{
				glm::mat4  model2(1.0f);
				model2 = glm::translate(model2, vegetation[i]);
				//面向摄像机
				glm::vec3 va(0.0f, 0.0f, -1.0f);
				glm::vec3 vb = normalize((mainCamera.Position - vegetation[i]));
				float a = (dot(vb, va)) / (glm::sqrt(va.x*va.x + va.y*va.y + va.z*va.z) * glm::sqrt(vb.x*vb.x + vb.y* vb.y + vb.z* vb.z));

				float b = glm::acos(a);
				if (b < 0)
				{
					b = -b;
				}

				//			cout << "a:" << a << "	-b:" << -b << endl;

				model2 = glm::rotate(model2, b, glm::vec3(0.0f, 1.0f, 0.0f));
				grassShader.setMat4("model", model2);

				Drawgrass(&grassShader, mainCamera);
			}

			//画地面
			modelShader.use();
			glm::mat4 floorMat(1.0f);
			floorMat = glm::translate(floorMat, zeroPos);
			floorMat = glm::scale(floorMat, glm::vec3(10.0f));
			modelShader.setMat4("model", floorMat);
			DrawFloor(&modelShader);

			//灯光设置
			lightShader.use();
			glBindVertexArray(lightVAO);

			for (const auto pointLightPosition : pointLightPositions)
			{
				glm::mat4 lightModel(1.0f);
				lightModel = glm::translate(lightModel, pointLightPosition);
				lightModel = glm::scale(lightModel, glm::vec3(0.2f));
				lightShader.setMat4("model", lightModel);
				lightShader.setMat4("view", view);
				lightShader.setMat4("proj", proj);

				lightShader.setVec3("lightColor", lightColor);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			//画原点
			glBindVertexArray(lightVAO);
			glm::mat4 lightModel(1.0f);
			lightModel = glm::translate(lightModel, zeroPos);
			lightModel = glm::scale(lightModel, glm::vec3(0.1f));
			lightShader.setMat4("model", lightModel);
			lightShader.setMat4("view", view);
			lightShader.setMat4("proj", proj);

			lightShader.setVec3("lightColor", glm::vec3(1.0f, 0.0f, 0.0f));

			glDrawArrays(GL_TRIANGLES, 0, 36);

			if (count == 1)
			{
				//第二次的始化画箱子，将整个场景作为箱子的纹理
				glBindVertexArray(VAO);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texColorBuffer);
				glm::mat4 rttModel(1.0f);
				rttModel = glm::translate(rttModel, glm::vec3(1.0f, 5.0f, 1.0f));
				rttModel = glm::scale(rttModel, glm::vec3(10.0f, 10.0f, 1.0f));
				rttShader.use();
				rttShader.setMat4("model", rttModel);
				rttShader.setMat4("view", view);
				rttShader.setMat4("proj", proj);
				rttShader.setInt("screenTexture", 0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			//画天空盒
			cubeMapShader.use();
			glm::mat4 m(1.0f);
			m = glm::scale(m, glm::vec3(10));
			glm::mat4 view2 = glm::mat4(glm::mat3(view));
			cubeMapShader.setMat4("view", view2);
			cubeMapShader.setMat4("proj", proj);
			cubeMapShader.setMat4("model", m);
			DrawCubeMap();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//交换缓冲
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//释放资源
	glfwTerminate();
	return 0;
}

void do_movement()
{
	// 摄像机控制
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		mainCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		mainCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		mainCamera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		mainCamera.ProcessKeyboard(RIGHT, deltaTime);

	if (keys[GLFW_KEY_UP])
	{
		moveF += 0.05f;
	}
	if (keys[GLFW_KEY_DOWN])
	{
		moveF -= 0.05f;
	}
	if (keys[GLFW_KEY_RIGHT])
	{
		moveR += 0.05f;
	}
	if (keys[GLFW_KEY_LEFT])
	{
		moveR -= 0.05f;
	}
	if (keys[GLFW_KEY_PAGE_UP])
	{
		moveU += 0.05f;
	}
	if (keys[GLFW_KEY_PAGE_DOWN])
	{
		moveU -= 0.05f;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	mainCamera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//	cout << yoffset << endl;
	mainCamera.ProcessMouseScroll(yoffset);
}

void windowSize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}