#pragma once
#ifndef  SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// 着色器程序ID
	unsigned int ID;

	// 读取并构建着色器
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	//	激活着色器程序
	void Active();

	//	uniform工具函数
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetFloat(const std::string& name, float x, float y, float z, float w = 1.0f) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec3(const std::string& name, const glm::vec3& pos);
	void SetMat4(const std::string& name, const glm::mat4& mat4);

private:
	void _checkCompileErrors(GLuint shader, std::string type);

};


#endif // ! SHADER_H
