#version 330 core
out vec4 FragColor;

float move;

void main()
{
    FragColor = vec4(1.0f * move, 0.5f, 0.2f, 1.0f);
} 