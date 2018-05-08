#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SpotLight
{
public:
	glm::vec3 Position = glm::vec3(0, 1, 0);
	glm::vec3 Direction = glm::vec3(0, -1, 0);
	glm::vec3 Color = glm::vec3(1.0f);
	float      Intensity = 1.0f;
	float      CutOff = glm::cos(glm::radians(1.5f));
	float       OuterCutOff = glm::cos(glm::radians(2.5f));
	bool       RenderMesh = true;
};