#version 330 core
out vec4 FragColor;

in vec4 vertexCol;

//uniform vec4 ourColor = vec4(1.0f , 0.5f, 1.0f, 1.0f);

uniform float green;
void main()
{
    FragColor = vec4(0.0f , green, 0.0f, 1.0f*green);
	//FragColor = ourColor;
} 