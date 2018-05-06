#version 330 core
#extension GL_ARB_shading_language_420pack : enable
layout (std140, binding = 0) uniform Global
{
    // trtansformations
    mat4 proj;//0
    mat4 view;//64*1
    // scene
    vec3 viewPos;//16
    // lighting
    vec3 dirLightDir[4];
    vec3 dirLightCol[4];

    vec3 pointLightPos[8];
    vec3 pointLightCol[8];

	vec3 spotLightPos[3];
	vec3 spotLightDir[3];
	vec3 spotLightCol[3];
	float spotLightCutOff[3];
	float spotLightOuterCutOff[3];

	int dirLightCount;
	int pointLightCount;
	int spotLightCount;

	float pointLightRadius[8];

};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;


uniform mat4 model;



void main()
{
    gl_Position =proj * view * model * vec4(aPos, 1.0f);
    //gl_Position =vec4(aPos, 1.0f);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	//Normal =  aNormal;
	TexCoords = aTexCoord;
	//Æ¬¶ÎÊÀ½ç×ø±ê
	FragPos = vec3(model * vec4(aPos,1.0f));
	
}