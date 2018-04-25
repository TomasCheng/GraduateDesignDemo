#version 330 core

out vec4 FragColor;

in vec3 textureDir; // ����3D��������ķ�������
uniform samplerCube cubemap; // ��������ͼ�����������

void main()
{             
    FragColor = texture(cubemap, textureDir);
    //FragColor = vec4(1.0f,0,1.0f,1.0f);
}