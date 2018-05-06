#version 330 core
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

//ƽ�й�
struct DirLight {
	vec3 direction;
    
	vec3 ambient ;
    vec3 diffuse ;
    vec3 specular ;
};  
DirLight dirLights[4];
//���Դ
struct PointLight {
    vec3 position;
	float radius;

	vec3 ambient ;
    vec3 diffuse ;
    vec3 specular ;
};  
PointLight pointLights[8];
//�۹�
struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;

	vec3 ambient ;
    vec3 diffuse ;
    vec3 specular ;
};
SpotLight spotLights[3];


in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

//���Ƭ����ɫ
out vec4 FragColor;

//#include common/Commom.glsl


//���мy��
uniform sampler2D TexAlbedo;
uniform sampler2D TexNormal;
uniform sampler2D TexMetallic;
uniform sampler2D TexRoughness;
uniform sampler2D TexAO;

uniform vec3 MainColor;



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // �ϲ����
    vec3 ambient  = light.ambient  * vec3(texture(TexAlbedo, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(TexAlbedo, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(TexMetallic, TexCoords));
    return (ambient + diffuse + specular);

}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // ˥��
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance)) * light.radius;    
    // �ϲ����
    vec3 ambient  = light.ambient  * vec3(texture(TexAlbedo, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(TexAlbedo, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(TexMetallic, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 norm,vec3 fragPos,vec3 viewDir)
{
	 // ������
    vec3 ambient = light.ambient  * vec3(texture(TexAlbedo,TexCoords));

    // ������ 
    vec3 lightDir = normalize(light.position - fragPos);

	//�۹�Ƶļ���
	float theta = dot(lightDir,normalize(-light.direction));

	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff)/epsilon,0.0f,1.0f);

	float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = light.diffuse  * diff * vec3(texture(TexAlbedo,TexCoords));

    // �����
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(TexMetallic,TexCoords));  

    vec3 result = ambient + diffuse * intensity + specular * intensity;
	return result;
}


void Init()
{
	for(int i=0;i<dirLightCount;i++){
		dirLights[i].direction = dirLightDir[i];
		dirLights[i].ambient = dirLightCol[i];
		dirLights[i].diffuse = dirLights[i].ambient * vec3(0.2);
		dirLights[i].specular = vec3(1.0);

	}
    for(int i = 0; i < pointLightCount; i++)
	{
		pointLights[i].position = pointLightPos[i];
		pointLights[i].ambient = pointLightCol[i];
		pointLights[i].radius = pointLightRadius[i];
		pointLights[i].diffuse = pointLights[i].ambient * vec3(0.2);
		pointLights[i].specular = vec3(1.0);
	}
	for(int i=0;i<spotLightCount;i++){
		spotLights[i].position = spotLightPos[i];
		spotLights[i].direction = spotLightDir[i];
		spotLights[i].ambient = spotLightCol[i];
		spotLights[i].cutoff = spotLightCutOff[i];
		spotLights[i].outerCutoff = spotLightOuterCutOff[i];
		spotLights[i].diffuse = spotLights[i].ambient * vec3(0.2);
		spotLights[i].specular = vec3(1.0);
	}
}


void main()
{
	Init();

	 //����
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result  = vec3(0);
    // ��һ�׶Σ��������
	for(int i=0;i<dirLightCount;i++){
		result += CalcDirLight(dirLights[i], norm, viewDir);
	}
    //FragColor = vec4(result, 1.0);

    // �ڶ��׶Σ����Դ
    for(int i = 0; i < pointLightCount; i++)
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // �����׶Σ��۹�
	for(int i=0;i<spotLightCount;i++){
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
	}

    FragColor = vec4(result * MainColor, 1.0);
	//FragColor = vec4(1,0,0,1);
} 