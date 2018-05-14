#pragma once
#pragma once

#include "glad/glad.h"

#include <map>
#include "Shader.h"
#include "ShaderTypes.h"

enum MaterialType
{
	MATERIAL_DEFAULT,
	MATERIAL_CUSTOM,
	MATERIAL_POST_PROCESS,
};

class Material
{
private:
	// shader state
	Shader * m_Shader;
	std::map<std::string, UniformValue>        m_Uniforms;
	std::map<std::string, UniformValueSampler> m_SamplerUniforms;
public:

	void Use();

	MaterialType Type = MATERIAL_CUSTOM;
	glm::vec4 Color = glm::vec4(1.0f);

	// depth state
	bool   DepthTest = true;
	bool   DepthWrite = true;
	GLenum DepthCompare = GL_LESS;

	// face culling state
	bool   Cull = true;
	GLenum CullFace = GL_BACK;
	GLenum CullWindingOrder = GL_CCW;

	// blending state
	bool   Blend = false;
	GLenum BlendSrc = GL_ONE; // pre-multiplied alpha
	GLenum BlendDst = GL_ONE_MINUS_SRC_ALPHA;
	GLenum BlendEquation = GL_FUNC_ADD;

	// shadow state
	bool ShadowCast = true;
	bool ShadowReceive = true;

private:

public:
	Material();
	Material(Shader* shader);

	Shader* GetShader();
	void    SetShader(Shader* shader);

	Material Copy();

	void SetBool(std::string name, bool value);
	void SetInt(std::string name, int value);
	void SetFloat(std::string name, float value);
	void SetTexture(std::string name, Texture* value, unsigned int unit = 0);
	void SetTextureCube(std::string name, TextureCube* value, unsigned int unit = 0);
	void SetVector(std::string name, glm::vec2 value);
	void SetVector(std::string name, glm::vec3 value);
	void SetVector(std::string name, glm::vec4 value);
	void SetMatrix(std::string name, glm::mat2 value);
	void SetMatrix(std::string name, glm::mat3 value);
	void SetMatrix(std::string name, glm::mat4 value);

	std::map<std::string, UniformValue>*        GetUniforms();
	std::map<std::string, UniformValueSampler>* GetSamplerUniforms();
};
