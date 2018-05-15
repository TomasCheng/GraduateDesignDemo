#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "Texture.h"
using namespace std;
enum TOPOLOGY
{
	POINTSM,
	LINES,
	LINE_STRIP,
	TRIANGLES,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
};

class Mesh
{
public:
	unsigned int m_VAO = 0;
	unsigned int m_VBO;
	unsigned int m_EBO;

	string Name = "defaultMeshName";
public:
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec2> UV;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec3> Tangents;
	std::vector<glm::vec3> Bitangents;

	TOPOLOGY Topology = TRIANGLES;
	std::vector<unsigned int> Indices;

	Mesh();
	Mesh(std::vector<glm::vec3> positions, std::vector<unsigned int> indices);
	Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<unsigned int> indices);
	Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<glm::vec3> normals, std::vector<unsigned int> indices);
	Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents, std::vector<unsigned int> indices);

	void SetPositions(std::vector<glm::vec3> positions);
	void SetUVs(std::vector<glm::vec2> uv);
	void SetNormals(std::vector<glm::vec3> normals);
	void SetTangents(std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents);

	void Finalize(bool interleaved = true);
};
