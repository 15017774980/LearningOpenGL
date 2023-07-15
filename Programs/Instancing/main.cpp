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
#include "Core/Texture/CubeTexture.h"

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
glm::mat4 _orthographic_projection_matrix = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 300.0f);
//透视投影变换矩阵
glm::mat4 _perspective_projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);

// 主摄像机
Camera mainCamera(glm::vec3(0.0f, 10.0f, 90.0f));

int main()
{
    Viewport::Instance()->SetBackColor(0.0f, 0.0f, 0.0f);

    glfwSetCursorPosCallback(Viewport::Instance()->GetWindow(), mouse_callback);
    glfwSetScrollCallback(Viewport::Instance()->GetWindow(), scroll_callback);
    glfwSetInputMode(Viewport::Instance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    /*float quadVertices[] = {
        // 位置          // 颜色
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f
    };

    unsigned int vbo, vao;
    {
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);
       
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        //glBindVertexArray(0);
    }

    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    Shader shader("Assets/shader/instancing/shader.vs", "Assets/shader/instancing/shader.fs");
    shader.Active();

    //for (unsigned int i = 0; i < 100; i++)
    //{
    //    shader.SetVec2(("offsets[" + std::to_string(i) + "]").c_str(), translations[i]);
    //}
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
  
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);
    */

    unsigned int amount = 50000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<float>(glfwGetTime()));
    float radius = 80.0f;
    float offset = 15.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1 位置
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2 缩放
        float scale = static_cast<float>((rand() % 20) / 100.0f + 0.05f);
        model = glm::scale(model, glm::vec3(scale));
        // 3 旋转
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        modelMatrices[i] = model;
    }

    Model planet("Assets/model/planet/planet.obj");
    Model rock("Assets/model/rock/rock.obj");

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < rock.meshs.size(); i++)
    {
        unsigned int vao = rock.meshs[i].VAO;
        glBindVertexArray(vao);
        //顶点属性
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    Shader planetShader("Assets/shader/instancing/planet_shader.vs", "Assets/shader/instancing/planet_shader.fs");
    Shader rockShader("Assets/shader/instancing/rock_shader.vs", "Assets/shader/instancing/rock_shader.fs");

    // 渲染循环
    while (!glfwWindowShouldClose(Viewport::Instance()->GetWindow()))
    {
        // 处理帧率与时间差
        tick_frame_delta_time();
        // 输入处理
        processInput(Viewport::Instance()->GetWindow());
        // 渲染指令
        Viewport::Instance()->PreRender();

        //glBindVertexArray(vao);
        //glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
        planetShader.Active();
        planetShader.SetMat4("projection", _perspective_projection_matrix);
        planetShader.SetMat4("view", mainCamera.GetViewMatrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f));
        planetShader.SetMat4("model", model);
        planet.Draw(planetShader);

        rockShader.Active();
        rockShader.SetMat4("projection", _perspective_projection_matrix);
        rockShader.SetMat4("view", mainCamera.GetViewMatrix());
        for (unsigned int i = 0; i < rock.meshs.size(); i++)
        {
            glBindVertexArray(rock.meshs[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, rock.meshs[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        }

        /*for (unsigned int i = 0; i < amount; i++)
        {
            rockShader.SetMat4("model", modelMatrices[i]);
            rock.Draw(rockShader);
        }*/

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
