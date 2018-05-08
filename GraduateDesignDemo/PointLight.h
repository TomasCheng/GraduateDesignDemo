#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PointLight
{
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Color = glm::vec3(1.0f);
	float      Intensity = 1.0f;
	float      Radius = 1.0f;
	bool       Visible = true;
	bool       RenderMesh = true;
};