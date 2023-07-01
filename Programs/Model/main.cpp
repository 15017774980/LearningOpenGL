#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Core/Shader/Shader.h"
#include "Core/Texture/Texture.h"
#include "Core/Camera/Camera.h"
#include "Core/Viewport.h"
#include "Core/Model/Model.h"
#include "Core/Light/DirectionLight.h"
#include <filesystem>

using namespace Core;

void processInput(GLFWwindow* window);
void processCameraInput(GLFWwindow* window);

void tick_frame_delta_time();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// 窗口宽高
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float deltaTime = 0.0f;
float lastTime = 0.0f;

//正交投影变换矩阵
glm::mat4 _orthographic_projection_matrix = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 100.0f);
//透视投影变换矩阵
glm::mat4 _perspective_projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

// 主摄像机
Camera mainCamera(glm::vec3(0.0f, 0.5f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

int main()
{
	Viewport::Instance()->SetBackColor(0.5f, 0.5f, 0.5f);
	Viewport::Instance()->SetSize(SCR_WIDTH, SCR_HEIGHT);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	Model model("Assets/model/nanosuit/nanosuit.obj");
	Shader shader("Assets/shader/obj_shader.vs", "Assets/shader/obj_shader.fs");
	shader.SetVec3("viewPos", mainCamera.Position);

	glfwSetInputMode(Viewport::Instance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(Viewport::Instance()->GetWindow(), mouse_callback);
	glfwSetScrollCallback(Viewport::Instance()->GetWindow(), scroll_callback);

	LighingParameters params;
	params.Ambient = glm::vec3(0.4f);
	params.Diffuse = glm::vec3(0.7f);
	params.Specular = glm::vec3(0.5f);
	DirectionLight dirLight(params);
	dirLight.Direction = glm::vec3(0.2f, -0.2f, -0.2f);

	// 渲染循环
	while (!glfwWindowShouldClose(Viewport::Instance()->GetWindow()))
	{
		// 处理帧率与时间差
		tick_frame_delta_time();
		// 输入处理
		processInput(Viewport::Instance()->GetWindow());
		// 渲染指令
		Viewport::Instance()->PreRender();
		shader.Active();
		glm::mat4 _view_matrix = mainCamera.GetViewMatrix();
		glm::mat4 _model_matrix = glm::mat4(1.0f);
		_model_matrix = glm::translate(_model_matrix, glm::vec3(0.0f, -1.6f, -0.5f));
		_model_matrix = glm::scale(_model_matrix, glm::vec3(0.2f));
		shader.SetMat4("model", _model_matrix);
		shader.SetMat4("view", _view_matrix);
		shader.SetMat4("projection", _perspective_projection_matrix);
		shader.SetVec3("viewPos", mainCamera.Position);

		dirLight.SetupLightToShader(shader, "dirLight");

		model.Draw(shader);

		// 检查并调用，交换缓冲
		Viewport::Instance()->SwapBuffers();
		glfwPollEvents();
		//glDepthMask(GL_FALSE);
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	processCameraInput(window);
}

void processCameraInput(GLFWwindow* window)
{
	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		mainCamera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
}

void tick_frame_delta_time()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastTime;
	lastTime = currentFrame;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mainCamera.ProcessMouseScroll(yoffset);
	_perspective_projection_matrix = glm::perspective(glm::radians(mainCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

float lastX = 400; float lastY = 300;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	mainCamera.ProcessMouseMovement(xoffset, yoffset);
}
