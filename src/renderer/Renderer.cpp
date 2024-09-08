#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "src/util/Exception.cpp"
#include "src/util/Shader.cpp"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

class Renderer {
protected:
    const char* name;
    unsigned int width;
    unsigned int height;

    GLFWwindow* window;

    const char* vertexShaderSource;
    const char* fragmentShaderSource;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int shaderProgram;

public:
    Renderer(const char* name, unsigned int width, unsigned int height, const char* vertexShaderSource, const char* fragmentShaderSource) {
        this->name = name;
        this->width = width;
        this->height = height;
        this->vertexShaderSource = vertexShaderSource;
        this->fragmentShaderSource = fragmentShaderSource;
    }

    unsigned int getWidth() {
        return width;
    }
    unsigned int getHeight() {
        return height;
    }

    void render() {
        initialize();
        generateBuffer();
        bindBuffer();
        attachShader();
        while (!glfwWindowShouldClose(window)) {
            processInput(window);
            drawElements();
        }
        clean();
    }

protected:
    void initialize() {
        // glfw: initialize and configure
        // ------------------------------
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            throw Exception(RENDERER_INITIALIZATION_EXCEPTION);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        // --------------------
        window = glfwCreateWindow(width, height, name, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw Exception(RENDERER_INITIALIZATION_EXCEPTION);
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw Exception(RENDERER_INITIALIZATION_EXCEPTION);
        }

        glEnable(GL_DEPTH_TEST);
    }

    virtual void generateBuffer() = 0;

    void bindBuffer() {
        // Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Vertex normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void attachShader() {
        // Compile shaders
        unsigned int vertexShader = Shader::compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        unsigned int fragmentShader = Shader::compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        // Link shaders to create a shader program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    virtual void drawElements() = 0;

    void clean() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        glfwTerminate();
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    virtual void processInput(GLFWwindow *window) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

};