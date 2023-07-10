#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT{
	vec2 texCoords;
	vec3 position;
	vec3 normal;
} vs_out;

uniform mat4 model;

void main()
{
	vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.texCoords = aTexCoords;
}