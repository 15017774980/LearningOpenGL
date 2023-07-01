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
#include <filesystem>

using namespace Core;

void processInput(GLFWwindow* window);
void processCameraInput(GLFWwindow* window);

void tick_frame_delta_time();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// 窗口宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	Viewport::Instance()->SetBackColor(0.9f, 0.9f, 0.9f);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	float matrixVertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	// 绘制矩阵（两个三角形组成的）
	unsigned vbo, vao;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader ourshader("Assets/shader/material_shader.vs", "Assets/shader/material_shader.fs");
	//Shader ourshader("Assets/shader/gouraud_light_shader.vs", "Assets/shader/gouraud_light_shader.fs");
	Shader DirectionLightShader("Assets/shader/light_shader.vs", "Assets/shader/direction_light_shader.fs");

	Texture tex0("Assets/texture/container2.png", TextureType::PNG);
	Texture tex1("Assets/texture/container2_specular.png", TextureType::PNG);

	//Texture emissionTex("Assets/texture/matrix.jpg", TextureType::JPG);

	ourshader.Active();
	ourshader.SetInt("material.diffuse",			0);
	ourshader.SetInt("material.specular",			1);
	ourshader.SetInt("material.emission",			2);
	ourshader.SetFloat("material.shininess",		128.0f);

	ourshader.SetVec3("viewPos", mainCamera.Position);

	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	//ourshader.SetVec3("light.position", lightPos);// .x, lightPos.y, lightPos.z, 0.0f);
	//ourshader.SetVec3("light.ambient",	0.1f, 0.1f, 0.1f);
	//ourshader.SetVec3("light.diffuse",		0.8f, 0.8f, 0.8f); // 将光照调暗了一些以搭配场景
	//ourshader.SetVec3("light.specular",	1.0f, 1.0f, 1.0f);

	//ourshader.SetFloat("light.constant",	1.0f);
	//ourshader.SetFloat("light.linear",		0.09f);
	//ourshader.SetFloat("light.quadratic", 0.032f);

	glfwSetInputMode(Viewport::Instance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(Viewport::Instance()->GetWindow(), mouse_callback);
	glfwSetScrollCallback(Viewport::Instance()->GetWindow(), scroll_callback);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 lightColor(1.0f);
	// 渲染循环
	while (!glfwWindowShouldClose(Viewport::Instance()->GetWindow()))
	{
		// 处理帧率与时间差
		tick_frame_delta_time();
		// 输入处理
		processInput(Viewport::Instance()->GetWindow());
		// 渲染指令
		Viewport::Instance()->PreRender();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1.ID);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionTex.ID);

		ourshader.Active();

		//glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 降低影响
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

// directional light
		ourshader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourshader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourshader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ourshader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		ourshader.SetVec3("pointLights[0].position", pointLightPositions[0]);
		ourshader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		ourshader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		ourshader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		ourshader.SetFloat("pointLights[0].constant", 1.0f);
		ourshader.SetFloat("pointLights[0].linear", 0.09f);
		ourshader.SetFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		ourshader.SetVec3("pointLights[1].position", pointLightPositions[1]);
		ourshader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		ourshader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		ourshader.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		ourshader.SetFloat("pointLights[1].constant", 1.0f);
		ourshader.SetFloat("pointLights[1].linear", 0.09f);
		ourshader.SetFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		ourshader.SetVec3("pointLights[2].position", pointLightPositions[2]);
		ourshader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		ourshader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		ourshader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		ourshader.SetFloat("pointLights[2].constant", 1.0f);
		ourshader.SetFloat("pointLights[2].linear", 0.09f);
		ourshader.SetFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		ourshader.SetVec3("pointLights[3].position", pointLightPositions[3]);
		ourshader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		ourshader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		ourshader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		ourshader.SetFloat("pointLights[3].constant", 1.0f);
		ourshader.SetFloat("pointLights[3].linear", 0.09f);
		ourshader.SetFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		ourshader.SetVec3("spotLight.position", mainCamera.Position);
		ourshader.SetVec3("spotLight.direction", mainCamera.Front);
		ourshader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ourshader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		ourshader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ourshader.SetFloat("spotLight.constant", 1.0f);
		ourshader.SetFloat("spotLight.linear", 0.09f);
		ourshader.SetFloat("spotLight.quadratic", 0.032f);
		ourshader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ourshader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0)));

		glBindVertexArray(vao);

		//观察矩阵（将场景朝反方向移动）
		glm::mat4 _view_matrix = mainCamera.GetViewMatrix();
		ourshader.SetMat4("view", _view_matrix);
		ourshader.SetMat4("projection", _perspective_projection_matrix);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			//模型矩阵
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourshader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(lightVAO);
		glm::mat4 _model_matrix = glm::mat4(1.0f);
		DirectionLightShader.Active();
		DirectionLightShader.SetMat4("view", _view_matrix);
		DirectionLightShader.SetMat4("projection", _perspective_projection_matrix);
		DirectionLightShader.SetVec3("LightColor", lightColor);
		for (unsigned int i = 0; i < 4; i++)
		{
			_model_matrix = glm::mat4(1.0f);
			_model_matrix = glm::translate(_model_matrix, pointLightPositions[i]);
			_model_matrix = glm::scale(_model_matrix, glm::vec3(0.2f));
			DirectionLightShader.SetMat4("model", _model_matrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// 检查并调用，交换缓冲
		Viewport::Instance()->SwapBuffers();
		glfwPollEvents();
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
