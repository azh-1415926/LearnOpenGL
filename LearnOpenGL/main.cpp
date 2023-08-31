#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string.h>

#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* 调整视口的回调函数 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /* 告诉OpenGL渲染窗口的尺寸大小 */
    glViewport(0, 0, width, height);
}

/* 当按下 Esc 键时请求退出窗口 */
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    /* 初始化 GLFW */
    glfwInit();
    /* 配置 GLFW，使用 3.3 版本的 OpenGL，主版本号(Major)和次版本号(Minor)都设置为 3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* 使用核心模式(Core-profile) */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Mac 系统需要取消下一行的注释 */
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* 创建窗口 */
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* 通知GLFW将 window 的上下文设置为当前线程的主上下文 */
    glfwMakeContextCurrent(window);

    /* 初始化 GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* 注册视口变化回调函数 */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /* 绘制三角形所用的顶点数据 */
    float vertices[] =
    {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] =
    {
        0,1,3,
        1,2,3
    };

    /* 创建并绑定顶点数组对象 */
    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    /* 创建并绑定顶点缓冲对象 */
    VertexBuffer vb(vertices, sizeof(vertices));
    vb.Bind();

    IndexBuffer ib(indices,6);
    ib.Bind();
    /* 告诉OpenGL该如何解析顶点数据 */
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    /* 创建一个着色器对象 */
    Shader shader("res/shaders/Basic.shader");
    float timeValue;
    float greenValue;

    /* 循环渲染，glfwWindowShouldClose 、检查 GLFW 是否被要求退出 */
    while (!glfwWindowShouldClose(window))
    {
        /* 设置用于清屏的颜色、清屏 */
        GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /* 输入 */
        processInput(window);

        /* 渲染 ... */
        shader.Bind();
        GLCall(glBindVertexArray(VAO));
        vb.Bind();
        ib.Bind();

        timeValue = glfwGetTime();
        greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shader.setUniform4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        /* 监听事件、交换缓冲 */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    /* 释放资源 */
    glfwTerminate();
    return 0;
}