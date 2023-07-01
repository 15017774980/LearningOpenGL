#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

void main()
{
	float ambientStregth = 0.1;
	vec3 ambient = ambientStregth * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(-FragPos);
	vec3 reflecDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflecDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 resulut = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(resulut, 1.0);
}