#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 textureDir;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

void main()
{
    textureDir = aPos;
    vec4 pos = proj * view * model * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}