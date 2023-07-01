#version 330 core
out vec4 FragColor;

struct Light{
	vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform Light dirLight;
uniform vec3 viewPos;

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));
    return (ambient + diffuse + specular);
}