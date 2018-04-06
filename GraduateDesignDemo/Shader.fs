#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float lerp;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    FragColor = vec4((lightColor * objectColor), 1.0f);
	//�����ǵ��������������������

	//vec4 c1 = texture(ourTexture1,TexCoord);
	//vec4 c2 = texture(ourTexture2,TexCoord);
	//c.a = 0.5;
	//FragColor = c * vec4(ourColor, 0.1f) ;
	//mixΪ��ֵ����
	//FragColor = mix(c1,c2,lerp);
	//FragColor = c1;

} 