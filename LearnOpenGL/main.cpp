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
    
    /* �������������õĶ������� */
    float vertices[] =
    {
        -0.5f, -0.5f ,0.0f,
         0.5f, -0.5f ,0.0f,
         0.0f,  0.5f ,0.0f
    };

    /* �������󶨶���������� */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* �������󶨶��㻺����� */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    /* ���������ݸ��Ƶ������� */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /* ����OpenGL����ν����������� */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* ����������ɫ����Ƭ����ɫ�� */
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderrSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

    /* ���붥����ɫ����Ƭ����ɫ�� */
    unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentShaderrSource, nullptr);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    /* ����һ���������Ӷ�����ɫ����Ƭ����ɫ������Ϊ��ɫ������ */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    /* ���ӵ����� */
    glLinkProgram(shaderProgram);
    /* �ͷ���ɫ�� */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* ѭ����Ⱦ��glfwWindowShouldClose ����� GLFW �Ƿ�Ҫ���˳� */
    while (!glfwWindowShouldClose(window))
    {
        /* ����������������ɫ������ */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* ���� */
        processInput(window);

        /* ��Ⱦ ... */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* �����¼����������� */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    /* �ͷ���Դ */
    glfwTerminate();
    return 0;
}