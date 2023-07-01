#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include "Core/Shader/Shader.h"
#include "Core/Texture/Texture.h"
#include "Core/Camera/Camera.h"
#include "Core/Viewport.h"
#include "Core/Model/Model.h"
#include "Core/Light/DirectionLight.h"

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
Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	Viewport::Instance()->SetBackColor(0.1f, 0.1f, 0.1f);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS);

    // 面剔除
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT); //剔除正向面

    // 启用混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	Shader shader("Assets/shader/depth_shader.vs", "Assets/shader/depth_shader.fs");
    Shader grassShader("Assets/shader/depth_shader.vs", "Assets/shader/grass_shader.fs");
    float cubeVertices[] = {
            // Back face
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
       // Front face
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
       // Left face
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
       // Right face
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
       // Bottom face
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
       // Top face
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        - 5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
         -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  1.0f
    };
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    unsigned int vegetationVAO, vegetationVBO;
    glGenVertexArrays(1, &vegetationVAO);
    glGenBuffers(1, &vegetationVBO);
    glBindVertexArray(vegetationVAO);
    glBindBuffer(GL_ARRAY_BUFFER ,vegetationVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    Texture cubeTexture("Assets/texture/marble.jpg");
    Texture floorTexture("Assets/texture/metal.png");
    Texture grassTexture("Assets/texture/blending_transparent_window.png");

    glfwSetInputMode(Viewport::Instance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Viewport::Instance()->GetWindow(), mouse_callback);
    glfwSetScrollCallback(Viewport::Instance()->GetWindow(), scroll_callback);

    vector<glm::vec3> windows;
    windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
    
    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < windows.size(); i++)
    {
        float distance = glm::length(mainCamera.Position - windows[i]);
        sorted[distance] = windows[i];
    }
	// 渲染循环
	while (!glfwWindowShouldClose(Viewport::Instance()->GetWindow()))
	{
		// 处理帧率与时间差
		tick_frame_delta_time();
		// 输入处理
		processInput(Viewport::Instance()->GetWindow());
		// 渲染指令
		Viewport::Instance()->PreRender();
		glm::mat4 _view_matrix = mainCamera.GetViewMatrix();
		glm::mat4 _model_matrix = glm::mat4(1.0f);
        grassShader.Active();
        grassShader.SetMat4("view", _view_matrix);
        grassShader.SetMat4("projection", _perspective_projection_matrix);
        shader.Active();
        shader.SetMat4("view", _view_matrix);
        shader.SetMat4("projection", _perspective_projection_matrix);
        shader.SetVec3("viewPos", mainCamera.Position);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture.ID);
        _model_matrix = glm::translate(_model_matrix, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.SetMat4("model", _model_matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        _model_matrix = glm::mat4(1.0f);
        _model_matrix = glm::translate(_model_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.SetMat4("model", _model_matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture.ID);
        shader.SetMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // grass
        grassShader.Active();
        glBindVertexArray(vegetationVAO);
        glBindTexture(GL_TEXTURE_2D, grassTexture.ID);
        /*for (unsigned int i = 0; i < windows.size(); i++)
        {
            _model_matrix = glm::mat4(1.0f);
            _model_matrix = glm::translate(_model_matrix, windows[i]);
            grassShader.SetMat4("model", _model_matrix);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }*/
        for (map<float, glm::vec3>::reverse_iterator it = sorted.rbegin();it != sorted.rend(); ++it)
        {
            _model_matrix = glm::mat4(1.0f);
            _model_matrix = glm::translate(_model_matrix, it->second);
            grassShader.SetMat4("model", _model_matrix);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);

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
