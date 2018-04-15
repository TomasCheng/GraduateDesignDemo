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

//�ƹ�
GLuint lightVAO;
glm::vec3 lightPos(0.0f, 2.3f, 0.0f);
glm::vec3 dynamicLightPos = lightPos;
Shader lightShader;
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

GLfloat deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
GLfloat lastFrame = 0.0f; // ��һ֡��ʱ��

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

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ��������
	int width, height, chanels;
	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &chanels, 0);
	// ��������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//�ͷ�ͼ����ڴ沢����������
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, texture);

	return texture;
}

void vertexObjectInit()
{
	//��ʹ������������������������λ���һ������
	// ���ö��㻺�������ָ��
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
	//���������������
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	// �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// ������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // �����������������ָ��ע�ᵽVBO��Ϊ��ǰ�󶨶������Ȼ�����ǾͿ��԰�ȫ�Ľ��
	glBindVertexArray(0); // ��� VAO (��ͨ����һ���ܺõ���������κλ���/���鲢��ֹ��ִ���ķ���)
}

int main()
{
	//��ʼ��GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//ע�ᴰ�ڴ�С�����仯ʱ�Ļص�
	glfwSetWindowSizeCallback(window, windowSize_callback);
	//ע����̻ص�
	glfwSetKeyCallback(window, key_callback);
	//ע�����ص�
	glfwSetCursorPosCallback(window, mouse_callback);
	//ע�������ֻص�
	glfwSetScrollCallback(window, scroll_callback);
	//���ù�����ز�����
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//��ʼ��GlAD

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad" << std::endl;
		return -1;
	}
	//����OpenGL��Ⱦ���ڳߴ��С
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	//	glDepthMask(GL_FALSE);

	//��ʼ������shaders
	shaderInit();
	//��ʼ�������������
	vertexObjectInit();
	//��ʼ���ƹ�VAO
	lightInit();

	Model nanoModel("nanosuit/nanosuit.obj");

	lightShader.use();
	lightShader.setVec3("lightColor", lightColor);

	mainCamera = Camera();

	//�����������������
	GLuint boxDiffuseMap = loadTexture("container2.jpg", GL_REPEAT, GL_LINEAR);
	GLuint boxSpecularMap = loadTexture("container2_specular.jpg", GL_REPEAT, GL_LINEAR);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(-6.0f, 1.5f, 0.0f),
		glm::vec3(-2.0f, 2.0f, 0.0f),
		glm::vec3(2.0f, 3.0f, 0.0f),
		glm::vec3(6.0f, 4.0f, 0.0f)
	};
	glm::vec3 zeroPos(0.0f);

	//����shader�е�һЩ������
	modelShader.use();
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.6f); //
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); //
	glm::vec3 specularColor = glm::vec3(1.0f);

	modelShader.setVec3("dirLight.ambient", ambientColor);
	modelShader.setVec3("dirLight.diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	modelShader.setVec3("dirLight.specular", specularColor);
	modelShader.setVec3("pointLights[0].ambient", ambientColor);
	modelShader.setVec3("pointLights[0].diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	modelShader.setVec3("pointLights[0].specular", specularColor);
	modelShader.setVec3("pointLights[1].ambient", ambientColor);
	modelShader.setVec3("pointLights[1].diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	modelShader.setVec3("pointLights[1].specular", specularColor);
	modelShader.setVec3("pointLights[2].ambient", ambientColor);
	modelShader.setVec3("pointLights[2].diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	modelShader.setVec3("pointLights[2].specular", specularColor);
	modelShader.setVec3("pointLights[3].ambient", ambientColor);
	modelShader.setVec3("pointLights[3].diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	modelShader.setVec3("pointLights[3].specular", specularColor);
	modelShader.setVec3("spotLight.ambient", ambientColor);
	modelShader.setVec3("spotLight.diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
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
	//���ʵ�����
	modelShader.setFloat("material.shininess", 32.0f);

	//�߿�shader
	Shader singleColorShader("Shader.vert", "SingleColorShader.frag");

	//�ô��ڽ������벢��������
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//����¼�
		glfwPollEvents();
		do_movement();

		glEnable(GL_DEPTH_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilMask(0x00);

		//���ø���ʱ��任��x��yƫ��ֵ������Ч��ΪԲ���˶�
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

		//		nanoModel.Draw(modelShader);

				//����ģ�建�����
		//		glEnable(GL_DEPTH_TEST);

		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilMask(0xff);

		//����
		glBindVertexArray(VAO);
		modelShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxDiffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, boxSpecularMap);
		modelShader.setInt("material.texture_diffuse0", 0);
		modelShader.setInt("material.texture_specular0", 1);
		modelShader.setInt("", 0);
		glm::mat4 boxMat(1.0f);
		boxMat = glm::translate(boxMat, glm::vec3(1.0f, 0.501f, 2.0f));
		modelShader.setMat4("model", boxMat);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ͨ����һ����һ������ӣ�ֻ���±߿�
		glBindVertexArray(VAO);
		singleColorShader.use();

		glStencilFunc(GL_NOTEQUAL, 1, 0xff);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		singleColorShader.setMat4("view", view);
		singleColorShader.setMat4("proj", proj);
		boxMat = glm::scale(boxMat, glm::vec3(1.04f, 1.04f, 1.04f));
		singleColorShader.setMat4("model", boxMat);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glStencilMask(0xff);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		//������
		modelShader.use();
		glm::mat4 floorMat(1.0f);
		floorMat = glm::translate(floorMat, zeroPos);
		floorMat = glm::scale(floorMat, glm::vec3(10.0f));
		modelShader.setMat4("model", floorMat);
		DrawFloor(&modelShader);

		//�ƹ�����
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

		//��ԭ��
		glBindVertexArray(lightVAO);
		glm::mat4 lightModel(1.0f);
		lightModel = glm::translate(lightModel, zeroPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.1f));
		lightShader.setMat4("model", lightModel);
		lightShader.setMat4("view", view);
		lightShader.setMat4("proj", proj);

		lightShader.setVec3("lightColor", glm::vec3(1.0f, 0.0f, 0.0f));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//��������
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//�ͷ���Դ
	glfwTerminate();
	return 0;
}

void do_movement()
{
	// ���������
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
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
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