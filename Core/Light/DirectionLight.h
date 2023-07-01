#pragma once
#include "LightBase.h"

class DirectionLight : public Light
{
public:
	DirectionLight(LighingParameters params)
		:Light(params)
	{
	}

	virtual void SetupLightToShader(Shader shader, const std::string& name)
	{
		// ·½Ïò
		shader.SetVec3((name + ".direction").c_str(), Direction);
		
		shader.SetVec3((name + ".ambient").c_str(), Parameters.Ambient);
		shader.SetVec3((name + ".diffuse").c_str(), Parameters.Diffuse);
		shader.SetVec3((name + ".specular").c_str(), Parameters.Specular);
	}

};