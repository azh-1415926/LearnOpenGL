#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
    
    /* ѭ����Ⱦ��glfwWindowShouldClose ����� GLFW �Ƿ�Ҫ���˳� */
    while (!glfwWindowShouldClose(window))
    {
        /* ����������������ɫ������ */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* ���� */
        processInput(window);

        /* ��Ⱦ ... */

        /* �����¼����������� */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    /* �ͷ���Դ */
    glfwTerminate();
    return 0;
}