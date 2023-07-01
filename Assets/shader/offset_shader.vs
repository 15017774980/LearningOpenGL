#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec4 offset;

out vec4 outColor;

void main()
{
	gl_Position = vec4(aPos + offset.xyz, 1.0);
	outColor = vec4(aPos, 1.0);
}