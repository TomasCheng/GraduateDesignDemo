#include "Material.h"
#include "Logger.h"
#include "ResourceLoader.h"

void Material::Use()
{
	if (this->m_Shader == nullptr)
	{
		Log::Message("null shader in material");
		return;
	}
	this->m_Shader->Use();

	auto* samplers = GetSamplerUniforms();
	for (auto it = samplers->begin(); it != samplers->end(); ++it)
	{
		if (it->second.Type == SHADER_TYPE_SAMPLERCUBE)
			it->second.TextureCube->Bind(it->second.Unit);
		else
			it->second.Texture->Bind(it->second.Unit);
	}

	auto* uniforms = GetUniforms();
	for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
	{
		switch (it->second.Type)
		{
		case SHADER_TYPE_BOOL:
			this->m_Shader->SetBool(it->first, it->second.Bool);
			break;
		case SHADER_TYPE_INT:
			this->m_Shader->SetInt(it->first, it->second.Int);
			break;
		case SHADER_TYPE_FLOAT:
			this->m_Shader->SetFloat(it->first, it->second.Float);
			break;
		case SHADER_TYPE_VEC2:
			this->m_Shader->SetVector(it->first, it->second.Vec2);
			break;
		case SHADER_TYPE_VEC3:
			this->m_Shader->SetVector(it->first, it->second.Vec3);
			break;
		case SHADER_TYPE_VEC4:
			this->m_Shader->SetVector(it->first, it->second.Vec4);
			break;
		case SHADER_TYPE_MAT2:
			this->m_Shader->SetMatrix(it->first, it->second.Mat2);
			break;
		case SHADER_TYPE_MAT3:
			this->m_Shader->SetMatrix(it->first, it->second.Mat3);
			break;
		case SHADER_TYPE_MAT4:
			this->m_Shader->SetMatrix(it->first, it->second.Mat4);
			break;
		default:
			Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
			break;
		case SHADER_TYPE_SAMPLER1D:
			break;
		case SHADER_TYPE_SAMPLER2D: break;
		case SHADER_TYPE_SAMPLER3D: break;
		case SHADER_TYPE_SAMPLERCUBE: break;
		}
	}
}

// --------------------------------------------------------------------------------------------
Material::Material()
{
}
// --------------------------------------------------------------------------------------------
Material::Material(Shader* shader)
{
	m_Shader = shader;
}
// --------------------------------------------------------------------------------------------
Shader* Material::GetShader()
{
	return m_Shader;
}
// --------------------------------------------------------------------------------------------
void Material::SetShader(Shader* shader)
{
	m_Shader = shader;
}
// --------------------------------------------------------------------------------------------
Material Material::Copy()
{
	Material copy(m_Shader);

	copy.Type = Type;

	copy.Color = Color;

	copy.DepthTest = DepthTest;
	copy.DepthWrite = DepthWrite;
	copy.DepthCompare = DepthCompare;

	copy.Cull = Cull;
	copy.CullFace = CullFace;
	copy.CullWindingOrder = CullWindingOrder;

	copy.Blend = Blend;
	copy.BlendSrc = BlendSrc;
	copy.BlendDst = BlendDst;
	copy.BlendEquation = BlendEquation;

	copy.m_Uniforms = std::map<std::string, UniformValue>();
	copy.m_SamplerUniforms = std::map<std::string, UniformValueSampler>();
	copy.SetVector("MainColor", glm::vec3(1));

	return copy;
}
// --------------------------------------------------------------------------------------------
void Material::SetBool(std::string name, bool value)
{
	m_Uniforms[name].Type = SHADER_TYPE_BOOL;
	m_Uniforms[name].Bool = value;
}
// --------------------------------------------------------------------------------------------
void Material::SetInt(std::string name, int value)
{
	m_Uniforms[name].Type = SHADER_TYPE_INT;
	m_Uniforms[name].Int = value;
}
// --------------------------------------------------------------------------------------------
void Material::SetFloat(std::string name, float value)
{
	m_Uniforms[name].Type = SHADER_TYPE_FLOAT;
	m_Uniforms[name].Float = value;
}
// --------------------------------------------------------------------------------------------
void Material::SetTexture(std::string name, Texture* value, unsigned int unit)
{
	if (value == nullptr)
	{
		Log::Message("material::SetTexture:" + name + "is nullptr", LOG_ERROR);
		return;
	}
	m_SamplerUniforms[name].Unit = unit;
	m_SamplerUniforms[name].Texture = value;

	switch (value->Target)
	{
	case GL_TEXTURE_1D:
		m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLER1D;
		break;
	case GL_TEXTURE_2D:
		m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLER2D;
		break;
	case GL_TEXTURE_3D:
		m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLER3D;
		break;
	case GL_TEXTURE_CUBE_MAP:
		m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLERCUBE;
		break;
	}

	if (m_Shader)
	{
		m_Shader->Use();
		m_Shader->SetInt(name, unit);
	}
}
// --------------------------------------------------------------------------------------------
void Material::SetTextureCube(std::string name, TextureCube* value, unsigned int unit)
{
	m_SamplerUniforms[name].Unit = unit;
	m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLERCUBE;
	m_SamplerUniforms[name].TextureCube = value;

	if (m_Shader)
	{
		m_Shader->Use();
		m_Shader->SetInt(name, unit);
	}
}
// ------------------------------------------------------------------------
void Material::SetVector(std::string name, glm::vec2 value)
{
	m_Uniforms[name].Type = SHADER_TYPE_VEC2;
	m_Uniforms[name].Vec2 = value;
}
// ------------------------------------------------------------------------
void Material::SetVector(std::string name, glm::vec3 value)
{
	m_Uniforms[name].Type = SHADER_TYPE_VEC3;
	m_Uniforms[name].Vec3 = value;
}
// ------------------------------------------------------------------------
void Material::SetVector(std::string name, glm::vec4 value)
{
	m_Uniforms[name].Type = SHADER_TYPE_VEC4;
	m_Uniforms[name].Vec4 = value;
}
// ------------------------------------------------------------------------
void Material::SetMatrix(std::string name, glm::mat2 value)
{
	m_Uniforms[name].Type = SHADER_TYPE_MAT2;
	m_Uniforms[name].Mat2 = value;
}
// ------------------------------------------------------------------------
void Material::SetMatrix(std::string name, glm::mat3 value)
{
	m_Uniforms[name].Type = SHADER_TYPE_MAT3;
	m_Uniforms[name].Mat3 = value;
}
// ------------------------------------------------------------------------
void Material::SetMatrix(std::string name, glm::mat4 value)
{
	m_Uniforms[name].Type = SHADER_TYPE_MAT4;
	m_Uniforms[name].Mat4 = value;
}
// ------------------------------------------------------------------------
std::map<std::string, UniformValue>*        Material::GetUniforms()
{
	return &m_Uniforms;
}
// ------------------------------------------------------------------------
std::map<std::string, UniformValueSampler>* Material::GetSamplerUniforms()
{
	return &m_SamplerUniforms;
}