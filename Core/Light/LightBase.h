#pragma once
#include <string>
#include <glm.hpp>
#include "../Shader/Shader.h"

struct LighingParameters
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Constant;
	float Linear;
	float Quadratic;
	float CutOff;
	float OuterCutOff;
};

class Light {
public:
	Light(LighingParameters params) 
		:Parameters(params)
	{
		Position = glm::vec3(0.0f);
		Direction = glm::vec3(0.0f);
	};

	virtual void SetupLightToShader(Shader shader, const std::string& name) = 0;

	LighingParameters Parameters;
	glm::vec3 Position;
	glm::vec3 Direction;
};