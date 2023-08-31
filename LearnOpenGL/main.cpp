#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string.h>

#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

/* �����ӿڵĻص����� */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /* ����OpenGL��Ⱦ���ڵĳߴ��С */
    glViewport(0, 0, width, height);
}

/* ������ Esc ��ʱ�����˳����� */
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    /* ��ʼ�� GLFW */
    glfwInit();
    /* ���� GLFW��ʹ�� 3.3 �汾�� OpenGL�����汾��(Major)�ʹΰ汾��(Minor)������Ϊ 3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* ʹ�ú���ģʽ(Core-profile) */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Mac ϵͳ��Ҫȡ����һ�е�ע�� */
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* �������� */
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* ֪ͨGLFW�� window ������������Ϊ��ǰ�̵߳��������� */
    glfwMakeContextCurrent(window);

    /* ��ʼ�� GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* ע���ӿڱ仯�ص����� */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /* �������������õĶ������� */
    float vertices[] =
    {
        0.5f, 0.5f, 0.0f,   // ���Ͻ�
        0.5f, -0.5f, 0.0f,  // ���½�
        -0.5f, -0.5f, 0.0f, // ���½�
        -0.5f, 0.5f, 0.0f   // ���Ͻ�
    };

    /* ���������±� */
    unsigned int indices[] =
    {
        0,1,3,
        1,2,3
    };

    /* �������󶨶���������� */
    VertexArray va;

    /* �������󶨶��㻺����� */
    VertexBuffer vb(vertices, sizeof(vertices));

    /* ������������������� */
    IndexBuffer ib(indices,6);

    /* �������岼�ֶ������û���ʱ 3 �� float ���ݻ���һ������ */
    VertexBufferLayout layout;
    layout.Push<float>(3);

    /* ��ӻ��嵽����������� */
    va.AddBuffer(vb, layout);

    /* ����һ����ɫ������ */
    Shader shader("res/shaders/Basic.shader");
    float timeValue;
    float greenValue;

    /* �������ƶ��� */
    Renderer renderer;
    /* ����������������ɫ */
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    /* ѭ����Ⱦ��glfwWindowShouldClose ����� GLFW �Ƿ�Ҫ���˳� */
    while (!glfwWindowShouldClose(window))
    {
        /* ����*/
        renderer.Clear();

        /* ���� */
        processInput(window);

        /* ��Ⱦ ... */
        timeValue = glfwGetTime();
        greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shader.setUniform4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        renderer.Draw(va, ib, shader);

        /* �����¼����������� */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    /* �ͷ���Դ */
    glfwTerminate();
    return 0;
}