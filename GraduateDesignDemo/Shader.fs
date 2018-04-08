#version 330 core

//���ʽṹ��
struct Material {
	sampler2D diffuse;
	sampler2D specular;
    float shininess;
}; 
struct Light {
	//�۹���ز���
    vec3 position;
	vec3 direction;
    float cutoff;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	//���Դ��˥��ϵ��
	//float constant;
	//float linear;
	//float quadratic;
};


uniform Material material;
uniform Light light;

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;


//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

uniform vec3 viewPos;


void main()
{
	//������Դ��˥��
	//float distance = length(light.position - FragPos);
	//float attenuation = 1.0f/(light.constant + light.linear * distance + light.quadratic*(distance * distance));


	

	    // ������
    vec3 ambient = light.ambient  * vec3(texture(material.diffuse,TexCoord));

    // ������ 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    //vec3 lightDir = normalize(-light.direction);

	//�۹�Ƶļ���
	float theta = dot(lightDir,normalize(-light.direction));

	if(theta > light.cutoff){
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse,TexCoord));

    // �����
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoord));  

    vec3 result = ambient + diffuse + specular;

    //FragColor = vec4(result * attenuation, 1.0);
    FragColor = vec4(result, 1.0);

	}else{
    FragColor = vec4(ambient, 1.0);
	
	}


    

	//�����ǵ��������������������

	//vec4 c1 = texture(ourTexture1,TexCoord);
	//vec4 c2 = texture(ourTexture2,TexCoord);
	//c.a = 0.5;
	//FragColor = c * vec4(ourColor, 0.1f) ;
	//mixΪ��ֵ����
	//FragColor = mix(c1,c2,lerp);
	//FragColor = c1;

} 