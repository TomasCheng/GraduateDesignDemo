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


#include "Camera.h"
using namespace std;
void loadModel(string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
//	directory = path.substr(0, path.find_last_of('/'));
//
//	processNode(scene->mRootNode, scene);
}

const GLuint WIDTH = 800, HEIGHT = 800;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint loadTexture(string fileName, GLint REPEAT, GLint FILTER);
void do_movement();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Camera mainCamera;
Shader shader; //shader
GLuint texContainer, texAwesomeface; //����id
GLuint diffuseMap;
GLuint specularMap;

float key_UD = 0.5f; //��ϱ���
GLuint VBO, VAO;

//�ƹ�
GLuint lightVAO;
glm::vec3 lightPos(0.0f, 1.3f, 0.0f);
glm::vec3 dynamicLightPos = lightPos;
Shader lightShader;
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
//glm::vec3 objectColor(1.0f, 0.0f, 0.0f);

GLfloat deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
GLfloat lastFrame = 0.0f; // ��һ֡��ʱ��

bool keys[1024];

GLfloat lastX = 400, lastY = 300;
GLfloat scrollSpeed = 0.05f;
bool firstMouse = true;

//
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void lightInit()
{
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
}

void shaderInit()
{
	shader = Shader("Shader.vs", "Shader.fs");
	lightShader = Shader("Shader.vs", "Light.fs");
}

void textureInit()
{
//	texContainer = loadTexture("container.jpg", GL_CLAMP_TO_EDGE, GL_LINEAR);
//	texAwesomeface = loadTexture("wall.jpg", GL_MIRRORED_REPEAT, GL_NEAREST);
	diffuseMap = loadTexture("container2.jpg", GL_MIRRORED_REPEAT, GL_NEAREST);
	specularMap = loadTexture("matrix.jpg", GL_MIRRORED_REPEAT, GL_NEAREST);
//	specularMap = loadTexture("container2.jpg", GL_MIRRORED_REPEAT, GL_NEAREST);
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
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f
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
	//��ʼ������shaders
	shaderInit();
	//��ʼ��textures
	textureInit();
	//��ʼ�������������
	vertexObjectInit();
	//��ʼ���ƹ�VAO
	lightInit();

	loadModel("nanosuit/nanosuit.obj");

	shader.use();
	shader.setVec3("lightColor", lightColor);
//	shader.setVec3("objectColor", objectColor);

	lightShader.use();
	lightShader.setVec3("lightColor", lightColor);

	mainCamera = Camera();

	glm::vec3 cubePositions[] = {
		glm::vec3(-10.0f, 0.0f, 0.0f),
		glm::vec3(-8.0f, 0.0f, 0.0f),
		glm::vec3(-6.0f, 0.0f, 0.0f),
		glm::vec3(-4.0f, 0.0f, 0.0f),
		glm::vec3(-2.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 0.0f, 0.0f),
		glm::vec3(4.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 0.0f, 0.0f),
		glm::vec3(8.0f, 0.0f, 0.0f),

	};

	//�ô��ڽ������벢��������
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//����¼�
		glfwPollEvents();
		do_movement();

		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//���ø���ʱ��任��x��yƫ��ֵ������Ч��ΪԲ���˶�
		GLfloat timeValue = glfwGetTime();
		GLfloat offsetx = 2 * ((sin(timeValue) / 2) + 0.5);
		GLfloat offsety = 2 * ((cos(timeValue) / 2) + 0.5);
//		dynamicLightPos.x = lightPos.x + offsetx;
//		dynamicLightPos.z = lightPos.z + offsety;
		
		//��������ͼ
		//		glActiveTexture(GL_TEXTURE0);
		//		glBindTexture(GL_TEXTURE_2D, texContainer);
		//		glUniform1i(glGetUniformLocation(shader.ID, "ourTexture1"), 0);
		//		glActiveTexture(GL_TEXTURE1);
		//		glBindTexture(GL_TEXTURE_2D, texAwesomeface);
		//		glUniform1i(glGetUniformLocation(shader.ID, "ourTexture2"), 1);

		// ����uniformֵ
		//�����˶��켣
		//GLint vertexorangeLocation = glGetUniformLocation(shader.Program, "offset");
		//glUniform4f(vertexorangeLocation, offsetx, offsety, 0.0f, 1.0f);
		//���û�ϱ���
		//		GLint mixPar = glGetUniformLocation(shader.ID, "lerp");
		//		glUniform1f(mixPar, key_UD);

//		glm::mat4 model0(1.0f);
//		model0 = glm::rotate(model0, (GLfloat)glfwGetTime() * 5.0f, glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view(1.0f);
		view = mainCamera.GetViewMatrix();

		glm::mat4 proj(1.0f);
		proj = glm::perspective(mainCamera.Zoom * scrollSpeed, (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

		shader.use();

//		shader.setMat4("model", model0);
		shader.setMat4("view", view);
		shader.setMat4("proj", proj);


//		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
//		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model0));
//		GLint viewLoc = glGetUniformLocation(shader.ID, "view");
//		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//		GLint projectionLoc = glGetUniformLocation(shader.ID, "proj");
//		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(VAO);
		//��һ��������
		shader.use();
		glm::mat4 model(1.0f);
		model = glm::translate(model, cubePositions[0]);
		shader.setMat4("model", model);
		shader.setVec3("viewPos", mainCamera.Position);

//		lightColor.x = sin(glfwGetTime() * 2.0f);
//		lightColor.y = sin(glfwGetTime() * 0.7f);
//		lightColor.z = sin(glfwGetTime() * 1.3f);

		shader.setVec3("lightColor", lightColor);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 
		glm::vec3 ambientColor = diffuseColor * glm::vec3(1.0f); // 

//		shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		shader.setFloat("material.shininess", 32.0f);

		shader.setVec3("light.ambient", ambientColor);
		shader.setVec3("light.diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
		shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//		shader.setVec3("light.position", dynamicLightPos);
		shader.setVec3("light.direction", -lightPos);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		shader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		shader.setInt("material.specular", 1);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);


		for(int i=1;i<10;i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

//		//�ڶ���
//		shader.setVec3("lightColor", lightColor);
//		shader.setVec3("lightPos", dynamicLightPos);
//		glm::mat4 model1(1.0f);
//		model = glm::translate(model1, cubePositions[1]);
//		shader.setMat4("model", model);
//		shader.setVec3("viewPos", mainCamera.Position);
//		glDrawArrays(GL_TRIANGLES, 0, 36);



		//�ƹ�����
		lightShader.use();
		glBindVertexArray(lightVAO);
		glm::mat4 lightModel(1.0f);
		lightModel = glm::translate(lightModel, dynamicLightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.setMat4("model", lightModel);
		lightShader.setMat4("view", view);
		lightShader.setMat4("proj", proj);

		lightShader.setVec3("lightColor", lightColor);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		
		


		//��������
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
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
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) //����UP�����ӻ�ϱ���
		key_UD = key_UD + 0.1f;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) //����DOWN��С��ϱ���
		key_UD = key_UD - 0.1f;

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
	cout << yoffset << endl;
	mainCamera.ProcessMouseScroll(yoffset);
}
