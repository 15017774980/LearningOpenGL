#pragma once
#include "LightBase.h"

class PointLight : public Light
{
public:
	virtual void SetupLightToShader(Shader shader, const std::string& name)
	{
		// ·½Ïò
		shader.SetVec3((name + ".position").c_str(), Position);

		shader.SetVec3((name + ".ambient").c_str(), Parameters.Ambient);
		shader.SetVec3((name + ".diffuse").c_str(), Parameters.Diffuse);
		shader.SetVec3((name + ".specular").c_str(), Parameters.Specular);
		shader.SetFloat((name + ".constant").c_str(), Parameters.Constant);
		shader.SetFloat((name + ".linear").c_str(), Parameters.Linear);
		shader.SetFloat((name + ".quadratic").c_str(), Parameters.Quadratic);
	}

};