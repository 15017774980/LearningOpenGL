#pragma once
#include "LightBase.h"

class SpotLight : public Light
{
public:
	virtual void SetupLightToShader(Shader shader, const std::string& name)
	{
		shader.SetVec3((name + ".position").c_str(), Position);
		shader.SetVec3((name + ".direction").c_str(), Direction);
		shader.SetVec3((name + ".ambient").c_str(), Parameters.Ambient);
		shader.SetVec3((name + ".diffuse").c_str(), Parameters.Diffuse);
		shader.SetVec3((name + ".specular").c_str(), Parameters.Specular);
		shader.SetFloat((name + ".constant").c_str(), Parameters.Constant);
		shader.SetFloat((name + ".linear").c_str(), Parameters.Linear);
		shader.SetFloat((name + ".quadratic").c_str(), Parameters.Quadratic);
		shader.SetFloat((name + ".cutOff").c_str(), Parameters.CutOff);
		shader.SetFloat((name + ".outerCutOff").c_str(), Parameters.OuterCutOff);
	}

};