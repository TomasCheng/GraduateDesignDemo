#version 330 core
#include Commom.glsl


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
	//Ƭ����������
	FragPos = vec3(model * vec4(aPos,1.0f));
	
}