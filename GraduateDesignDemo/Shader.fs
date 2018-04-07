#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

in vec3 FragPos;


uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float lerp;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;

uniform vec3 objectColor;

void main()
{
	//环境光
	float ambientStrenth = 0.1f;
	vec3 ambient  = ambientStrenth * lightColor;
	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0f);
	vec3 diffuse = diff * lightColor;
	//高光
	float specularStrength = 0.5f;
	int shininess = 32;//反光度
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(reflectDir,viewDir),0.0f),shininess);
	vec3 specular = spec * specularStrength * lightColor;


	vec3 result = (ambient + diffuse + specular)* objectColor;

    FragColor = vec4(result, 1.0f);
	//用我们的纹理坐标来对纹理采样

	//vec4 c1 = texture(ourTexture1,TexCoord);
	//vec4 c2 = texture(ourTexture2,TexCoord);
	//c.a = 0.5;
	//FragColor = c * vec4(ourColor, 0.1f) ;
	//mix为插值函数
	//FragColor = mix(c1,c2,lerp);
	//FragColor = c1;

} 