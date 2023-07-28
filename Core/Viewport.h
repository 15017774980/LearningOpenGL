#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "Singleton.h"

namespace Core
{
	class Viewport: public Singleton<Viewport>
	{
	public:
		Viewport() {
			_setup();
		};

		Viewport(char* name)
			:__name(name)
		{
			_setup();
		}

		Viewport(char* name, int width, int height)
			:__name(name),
			__width(width),
			__height(height)
		{
			_setup();
		}

		GLFWwindow* GetWindow()	{	return __window;		}

		void PreRender()
		{
			glClearColor(__backColor[0], __backColor[1], __backColor[2], 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void SwapBuffers() { glfwSwapBuffers(__window); }
		void SetBackColor(float a, float r, float b)
		{
			__backColor[0] = a;
			__backColor[1] = r;
			__backColor[2] = b;
		}
		void SetSize(int width, int height)
		{
			__width = width;
			__height = height;
			glViewport(0, 0, width, height);
		}

	private:
		int __width = 1280;
		int __height = 720;
		std::string __name = "Viewport";
		float __backColor[3] = { 0.2f, 0.3f, 0.3f };

		GLFWwindow* __window = NULL;

	private:
		static void _onSize(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void _setup()
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//	配置主版本号 3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	//	配置此版本号 3
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//	配置核心模式
			//glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

			__window = glfwCreateWindow(__width, __height, __name.c_str(), NULL, NULL);
			if (__window == NULL)
			{
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return;
			}
			glfwMakeContextCurrent(__window);
			glfwSetFramebufferSizeCallback(__window, Viewport::_onSize);

			// 初始化 GLAD
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
				return;
			}
		}
	};
}