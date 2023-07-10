#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform samplerCube skybox;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;

void main()
{
	vec3 diffuse = vec3(texture(texture_diffuse1, TexCoords));
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	vec3 refl = vec3(texture(texture_reflection1, TexCoords));
	vec3 reflection = refl * vec3(texture(skybox, R));
	vec4 result = vec4(diffuse + reflection, 1.0);
	FragColor = result;
}
