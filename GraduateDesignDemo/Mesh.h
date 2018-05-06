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

/*

Base mesh class. A mesh in its simplest form is purely a list of vertices, with some added
functionality for easily setting up the hardware configuration relevant for rendering.

*/
class Mesh
{
	// NOTE(Joey): public for now for testing and easy access; will eventually be private and only visible to renderer (as a friend class)
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

	// support multiple ways of initializing a mesh
	Mesh();
	Mesh(std::vector<glm::vec3> positions, std::vector<unsigned int> indices);
	Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<unsigned int> indices);
	Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<glm::vec3> normals, std::vector<unsigned int> indices);
	Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents, std::vector<unsigned int> indices);

	// set vertex data manually
	void SetPositions(std::vector<glm::vec3> positions);
	void SetUVs(std::vector<glm::vec2> uv);
	void SetNormals(std::vector<glm::vec3> normals);
	void SetTangents(std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents); // NOTE(Joey): you can only set both tangents and bitangents at the same time to prevent mismatches

																						  // commits all buffers and attributes to the GPU driver
	void Finalize(bool interleaved = true);
};
