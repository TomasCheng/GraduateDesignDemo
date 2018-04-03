#version 330 core
out vec4 FragColor;

in vec3 ourColor;

in vec3 p;
void main()
{
    FragColor = vec4(p, 1.0f);
} 