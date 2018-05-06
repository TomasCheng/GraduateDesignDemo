#ifndef COMMOM_H
#define COMMOM_H
#extension GL_ARB_shading_language_420pack : enable
// global uniform buffer for shared common set of uniforms among programs

layout (std140, binding = 0) uniform Global
{
    // trtansformations
    mat4 proj;
    mat4 view;
    // scene
    vec4 viewPos;
    // lighting
    vec4 dirLightDir[4];
    vec4 dirLightCol[4];

    vec4 pointLightPos[8];
    vec4 pointLightCol[8];

	vec4 spotLightPos[3];
	vec4 spotLightDir[3];
	vec4 spotLightCol[3];
	float spotLightCutOff[3];
	float spotLightOuterCutOff[3];

	int dirLightCount;
	int pointLightCount;
	int spotLightCount;

	float pointLightRadius[8];

};



#endif