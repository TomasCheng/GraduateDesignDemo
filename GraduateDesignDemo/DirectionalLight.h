#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class DirectionalLight
{
public:
	glm::vec3 Direction = glm::vec3(-1.0);
	glm::vec3 Color = glm::vec3(1.0f);
	float Intensity = 1.0f;

	bool CastShadows = true;
};