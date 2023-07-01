#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec3 LightingColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vec3 positon = vec3(view * model * vec4(aPos, 1.0));
	vec3 normal = mat3(transpose(inverse(view * model))) * aNormal;
	vec3 lPos = vec3(view * vec4(lightPos, 1.0));

	float ambientStregth = 0.1;
	vec3 ambient = ambientStregth * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lPos - positon);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(-positon);
	vec3 reflecDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflecDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	LightingColor = ambient + diffuse + specular;
}