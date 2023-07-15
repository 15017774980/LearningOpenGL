#include "Shader.h"
#include <gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{

    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
	std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
	std::ifstream gShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
		if (geometryPath != nullptr)
		{
			// 打开文件
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// 创建顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);//	创建着色器
	glShaderSource(vertex, 1, &vShaderCode, NULL);// 绑定着色器代码
	glCompileShader(vertex);// 编译着色器
	//	检查顶点着色器是否编译成功
	_checkCompileErrors(vertex, "VERTEX");

	// 创建片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	_checkCompileErrors(vertex, "FRAGMENT");

	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		// 创建几何着色器
		geometry = glCreateShader(GL_GEOMETRY_SHADER);//	创建着色器
		glShaderSource(geometry, 1, &gShaderCode, NULL);// 绑定着色器代码
		glCompileShader(geometry);// 编译着色器
		_checkCompileErrors(geometry, "GEOMETRY");
	}

	// 创建着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);	//附加顶点着色器
	glAttachShader(ID, fragment);	//附加片段着色器
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	_checkCompileErrors(vertex, "PROGRAM");

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}

void Shader::Active()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& val) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &val[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& pos) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &pos[0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat4) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::_checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
