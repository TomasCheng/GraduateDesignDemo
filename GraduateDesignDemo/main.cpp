#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include "Camera.h"
#include "Floor.h"
#include "Grass.h"
#include "CubeMap.h"
#include "ResourceLoader.h"
#include "Material.h"
#include "CubeMesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Torus.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "TextRender.h"
#include <sstream>
using namespace std;

const GLuint WIDTH = 1080, HEIGHT = 1080;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint loadTexture(string fileName, GLint REPEAT, GLint FILTER);
void do_movement();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void windowSize_callback(GLFWwindow* window, int width, int height);

//Camera mainCamera;
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

std::stringstream fpsss;
std::string fpsstr;

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
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	glEnable(GL_MULTISAMPLE);

	Scene::mainCamera = new Camera;

	glm::vec3 pointLightPositions[] = {
		glm::vec3(-6.0f, 1.5f, 0.0f),
		glm::vec3(-2.0f, 2.0f, 0.0f),
		glm::vec3(2.0f, 3.0f, 0.0f),
		glm::vec3(6.0f, 4.0f, 0.0f)
	};
	glm::vec3 zeroPos(0.0f);

	//	//����shader�е�һЩ������
	//	modelShader.use();
	//	glm::vec3 diffuseColor = lightColor * glm::vec3(0.6f); //
	//	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); //
	//	glm::vec3 specularColor = glm::vec3(1.0f);
	//
	//	modelShader.setVec3("dirLight.ambient", ambientColor);
	//	modelShader.setVec3("dirLight.diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	//	modelShader.setVec3("dirLight.specular", specularColor);
	//	modelShader.setVec3("pointLights[0].ambient", ambientColor);
	//	modelShader.setVec3("pointLights[0].diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	//	modelShader.setVec3("pointLights[0].specular", specularColor);
	//	modelShader.setVec3("pointLights[1].ambient", ambientColor);
	//	modelShader.setVec3("pointLights[1].diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	//	modelShader.setVec3("pointLights[1].specular", specularColor);
	//	modelShader.setVec3("pointLights[2].ambient", ambientColor);
	//	modelShader.setVec3("pointLights[2].diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	//	modelShader.setVec3("pointLights[2].specular", specularColor);
	//	modelShader.setVec3("pointLights[3].ambient", ambientColor);
	//	modelShader.setVec3("pointLights[3].diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	//	modelShader.setVec3("pointLights[3].specular", specularColor);
	//	modelShader.setVec3("spotLight.ambient", ambientColor);
	//	modelShader.setVec3("spotLight.diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
	//	modelShader.setVec3("spotLight.specular", specularColor);
	//
	//	modelShader.setVec3("dirLight.direction", -lightPos);
	//
	//	modelShader.setVec3("spotLight.position", mainCamera.Position);
	//	modelShader.setVec3("spotLight.direction", mainCamera.Front);
	//	modelShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
	//	modelShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));
	//
	//	modelShader.setVec3("spotLight[0].position", pointLightPositions[0]);
	//	modelShader.setFloat("pointLights[0].constant", 1.0f);
	//	modelShader.setFloat("pointLights[0].linear", 0.09f);
	//	modelShader.setFloat("pointLights[0].quadratic", 0.032f);
	//	modelShader.setVec3("spotLight[1].position", pointLightPositions[1]);
	//	modelShader.setFloat("pointLights[1].constant", 1.0f);
	//	modelShader.setFloat("pointLights[1].linear", 0.09f);
	//	modelShader.setFloat("pointLights[1].quadratic", 0.032f);
	//	modelShader.setVec3("spotLight[2].position", pointLightPositions[2]);
	//	modelShader.setFloat("pointLights[2].constant", 1.0f);
	//	modelShader.setFloat("pointLights[2].linear", 0.09f);
	//	modelShader.setFloat("pointLights[2].quadratic", 0.032f);
	//	modelShader.setVec3("spotLight[3].position", pointLightPositions[3]);
	//	modelShader.setFloat("pointLights[3].constant", 1.0f);
	//	modelShader.setFloat("pointLights[3].linear", 0.09f);
	//	modelShader.setFloat("pointLights[3].quadratic", 0.032f);
	//
	//	modelShader.use();
	//	//���ʵ�����
	//	modelShader.setFloat("material.shininess", 32.0f);
	//
	//	vector<glm::vec3> vegetation;
	//	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	//	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	//	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	//	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	//	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
	//
	//	Shader grassShader("Shader.vert", "GrassShader.frag");

	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// ��������
	glActiveTexture(GL_TEXTURE0);
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	//	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texColorBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, WIDTH, HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// �������ӵ���ǰ�󶨵�֡�������
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texColorBuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, rbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//	Shader rttShader = Shader("RTTShader.vert", "RTTShader.frag");
	//
	//	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ
	//
	//	//��պ�
	//	Shader cubeMapShader = Shader("cubeMapShader.vert", "cubeMapShader.frag");

	glDepthFunc(GL_LEQUAL);

	Scene::Init();
	TextRender::Init(WIDTH, HEIGHT);

	//��������
	Scene::SoundPlayer->play2D("ophelia.mp3", true);

	DirectionalLight* directional_light = new DirectionalLight();
	directional_light->Color = glm::vec3(0.9);
	Scene::AddLight(directional_light);

	PointLight* pointLight = new PointLight();
	pointLight->Color = glm::vec3(1, 1, 0);
	pointLight->Position = glm::vec3(-7, 1.5, 0);
	Scene::AddLight(pointLight);

	PointLight* pointLight2 = new PointLight();
	pointLight2->Color = glm::vec3(1, 0, 0);
	pointLight2->Position = glm::vec3(10, 1.5, 0);
	Scene::AddLight(pointLight2);

	//	Shader* shader = ResourceLoader::LoadShader("test");
	//	Material* material = new Material(shader);
	//	Mesh* mesh = new Cube;
	//	SceneNode* node = new SceneNode(mesh, material);
	//	material->SetVector("MainColor", glm::vec3(1.0));
	//	Texture* defaultTexAlbedo = ResourceLoader::LoadTexture("default", "container2.jpg");
	//	Texture* defaultTexMetallic = ResourceLoader::LoadTexture("default", "container2_specular.jpg");
	//	material->SetTexture("TexAlbedo", defaultTexAlbedo, 0);
	//	material->SetTexture("TexMetallic", defaultTexMetallic, 2);
	//	node->SetPosition(glm::vec3(2, 2, 0));
		//
		//	mesh* plane = new Plane(16, 16);
		//	material* material2 = new material(shader);
		//	SceneNode* node2 = Scene::MakeSceneNode(plane, material2);

			//	material* mat = Scene::GetDefaultMaterialCopy();
			//	mesh* cube = new Cube;
			//	SceneNode* node3 = Scene::MakeSceneNode(cube, mat);

	Shader* s1 = ResourceLoader::LoadShader("floor");
	Material* m1 = new Material(s1);
	//	Texture* t1 = ResourceLoader::LoadTexture("floor", "mesh/robo/textures/rcs-naofield.png");
	Texture* t1 = ResourceLoader::LoadTexture("floor", "container2.jpg");
	m1->SetTexture("TexAlbedo", t1, 0);
	//	m1->SetTexture("TexMetallic", t1, 2);
	m1->SetVector("MainColor", glm::vec3(1.0));
	Mesh* mesh1 = new Plane(1, 1);
	SceneNode* n1 = new SceneNode(mesh1, m1);
	n1->SetPosition(glm::vec3(-1, 0, 0));
	n1->SetRotation(glm::vec4(1, 0, 0, glm::radians(90.0f)));
	n1->SetScale(glm::vec3(65, 1, 65));

	Shader* s2 = ResourceLoader::LoadShader("wall");
	Material* m2 = new Material(s1);
	//	Texture* t1 = ResourceLoader::LoadTexture("floor", "mesh/robo/textures/rcs-naofield.png");
	Texture* t2 = ResourceLoader::LoadTexture("floor", "container2.jpg");
	m2->SetTexture("TexAlbedo", t1, 0);
	//	m1->SetTexture("TexMetallic", t1, 2);
	m2->SetVector("MainColor", glm::vec3(1.0));
	Mesh* mesh2 = new Plane(1, 1);
	SceneNode* n2 = new SceneNode(mesh1, m1);
	n2->SetPosition(glm::vec3(-10, 0, 0));
	//	n2->SetRotation(glm::vec4(1, 0, 0, glm::radians(90.0f)));
	n2->SetScale(glm::vec3(65, 65, 65));

	//	SceneNode * n2 = ResourceLoader::LoadMesh("Model", "mesh/nanosuit/nanosuit.obj");
	//	n2->SetPosition(glm::vec3(0, 0, 0));

//	SceneNode * n3 = ResourceLoader::LoadMesh("sponza", "mesh/sponza/sponza.obj");
//	n3->SetPosition(glm::vec3(300, 0, 0));
//	n3->SetScale(0.1f);

	//	SceneNode * n4 = ResourceLoader::LoadMesh("Model", "mesh/robo/models/naobody.obj");
	//	n4->SetPosition(glm::vec3(0, 0, 4));
		//	Scene::AddChild(n2);

		//�ٷֳ�2������
		//�˶��������۹��
	SpotLight* spot0 = new SpotLight();
	spot0->Position = glm::vec3(0, 5, 0);
	spot0->Direction = glm::vec3(0, -1, 0);
	spot0->Color = glm::vec3(1, 1, 0);
	Scene::AddLight(spot0);

	TextureCube* skybox = ResourceLoader::LoadTextureCube("skybox", "");
	//	Scene::SetSkyBox(skybox);

	Scene::PrintNodeTree(Scene::Root);

	//�ô��ڽ������벢��������
	while (!glfwWindowShouldClose(window))
	{
		//		for (int count = 0; count < 2; count++)
		//		{
		//			if (count == 0)
		//			{
		//				//				glViewport(0, 0, width, height);
		//
		//				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//			}
		//			else
		//			{
		//				//				glViewport(0, 0, 10.0f, 10.0f);
		//
		//				glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//			}

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
		dynamicLightPos.x = lightPos.x + offsetx;
		dynamicLightPos.z = lightPos.z + offsety;

		glm::mat4 view(1.0f);
		view = Scene::mainCamera->GetViewMatrix();
		glm::mat4 proj(1.0f);
		proj = glm::perspective(Scene::mainCamera->Zoom * scrollSpeed, (float)(WIDTH / HEIGHT), 0.1f, 1000.0f);

		Scene::mainCamera->UpdateProj(proj);

		spot0->Position = Scene::mainCamera->Position;
		spot0->Direction = Scene::mainCamera->Front;

		Scene::Update();

		//��FPS�Ĵ���
		double fps = 1.0f / deltaTime;
		fpsss.str("");
		fps = ((double)((int)((fps + 0.005) * 100))) / 100;
		fpsss << fps;
		fpsstr = fpsss.str();
		TextRender::Render("FPS:" + fpsstr, WIDTH - 200, HEIGHT - 30, 0.5, glm::vec3(1, 0.0f, 0.0f));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		Scene::mainCamera->ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		Scene::mainCamera->ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		Scene::mainCamera->ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		Scene::mainCamera->ProcessKeyboard(RIGHT, deltaTime);

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

	Scene::mainCamera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//	cout << yoffset << endl;
	Scene::mainCamera->ProcessMouseScroll(yoffset);
}

void windowSize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}