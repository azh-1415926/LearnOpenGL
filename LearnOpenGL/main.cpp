#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

/* �洢������ɫ����Ƭ����ɫ�� */
struct ShaderSource
{
    std::string vertexShader;
    std::string fragmentShader;
};

/* ��ȡ�ļ��е���ɫ�� */
ShaderSource ParseShaderSource(const std::string& filepath)
{
    std::ifstream stream(filepath);
    enum ShaderType
    {
        NONE = -1, VERRTEX = 0, FRAGMENT = 1
    };
    ShaderType type=ShaderType::NONE;
    std::stringstream ss[2];
    std::string line;
    while (getline(stream,line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERRTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";;
        }
    }
    return { ss[0].str(),ss[1].str() };
}

/* ������ɫ�� */
unsigned int CompileShader(const std::string& source,unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* str = source.c_str();
    glShaderSource(id, 1, &str, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(sizeof(char) * length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

/* ������ɫ������ */
unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

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

    /* ����һ����ɫ������ */
    ShaderSource source = ParseShaderSource("res/shaders/Basic.shader");
    unsigned int shaderProgram = CreateShader(source.vertexShader,source.fragmentShader);

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