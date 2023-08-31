#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

/* 存储顶点着色器、片段着色器 */
struct ShaderSource
{
    std::string vertexShader;
    std::string fragmentShader;
};

/* 读取文件中的着色器 */
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

/* 编译着色器 */
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

/* 创建着色器程序 */
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
        -0.5f, -0.5f ,0.0f,
         0.5f, -0.5f ,0.0f,
         0.0f,  0.5f ,0.0f
    };

    /* 创建并绑定顶点数组对象 */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* 创建并绑定顶点缓冲对象 */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    /* 将顶点数据复制到缓冲区 */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /* 告诉OpenGL该如何解析顶点数据 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* 创建一个着色器程序 */
    ShaderSource source = ParseShaderSource("res/shaders/Basic.shader");
    unsigned int shaderProgram = CreateShader(source.vertexShader,source.fragmentShader);

    /* 循环渲染，glfwWindowShouldClose 、检查 GLFW 是否被要求退出 */
    while (!glfwWindowShouldClose(window))
    {
        /* 设置用于清屏的颜色、清屏 */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* 输入 */
        processInput(window);

        /* 渲染 ... */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* 监听事件、交换缓冲 */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    /* 释放资源 */
    glfwTerminate();
    return 0;
}