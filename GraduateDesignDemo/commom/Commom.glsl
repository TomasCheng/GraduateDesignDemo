#ifndef COMMOM_H
#define COMMOM_H
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



#endif