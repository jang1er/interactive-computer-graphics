#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace glframework
{
    struct vertex
    {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
        glm::vec3 color;
    };

    struct texture
    {
        GLuint id;
        GLint width;
        GLint height;
    };

    //
    // Data Loading Functions
    //

    static const char* loadShaderSource(char const* path);
    static unsigned char* loadImageData(char const* path, int* width, int* height);
    static std::vector<vertex> loadOBJVertices(char const* path);

    //
    // OpenGL Helper Funtions
    // 
        
    struct vao
    {
        GLuint id;
        GLuint vbo;
        GLuint vertexCount;
    };

    vao createVertexArrayObject(std::vector<vertex> vertices);

    GLuint compileShader(GLuint type, const GLchar* shaderSource);
    GLuint linkShaderProgram(GLuint vertexShader, GLuint fragmentShader);
    GLuint loadShaderProgram(const char* vertShaderSourceFile, const char* fragShaderSourceFile);

    //
    // Framework Interface Functions
    //

    bool init(const char* WindowName);
    void destroy();
    void beginFrame();
    void endFrame();
    bool isRunning();
    glm::vec2 getWindowSize();
    glm::mat4 getCamera();
    float getTime();

    typedef void (*mouse_callback_func)(glm::vec2 mousePos);
    typedef void (*scroll_callback_func)(float scroll, glm::vec2 mousePos);

    void setMouseCallback(mouse_callback_func);
    void setScrollCallback(scroll_callback_func);
}

//
// Implementation
//

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define OBJLOADER_IMPLEMENTATION
#include <objloader.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace glframework
{
    static struct {
        GLFWwindow* window;
        bool mouseDown;
        glm::vec2 mousePos;
        glm::vec3 cameraPos;

        mouse_callback_func mouseCallback;
        scroll_callback_func scrollCallback;
    } globalState;

    static const char* loadShaderSource(char const* path)
    {
        std::ifstream inputStream(path);
        if (!inputStream.is_open()) {
            std::cerr << "Could not read file " << path << ". File does not exist." << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << inputStream.rdbuf();
        std::string sourceStr = buffer.str();
        char* source = new char[sourceStr.length() + 1];
        strcpy(source, sourceStr.c_str());
        return source;
    }

    static unsigned char* loadImageData(char const* path, int* width, int* height)
    {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path, width, height, 0, 4);
        return data;
    }

    texture loadTexture(const char* filename)
    {
        // create texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load texture data and generate mipmaps
        int width, height;
        unsigned char* data = glframework::loadImageData(filename, &width, &height);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        free(data);

        return { texture, width, height };
    }

    static std::vector<vertex> loadOBJVertices(char const* path)
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals; // Won't be used at the moment.
        bool res = loadOBJ("res/cube.obj", positions, texcoords, normals);
        if (!res) return {};

        std::vector<vertex> vertices(positions.size());
        for (int i = 0; i < vertices.size(); ++i)
        {
            vertices[i].position = positions[i];
            vertices[i].texcoord = texcoords[i];
            vertices[i].normal = normals[i];
            vertices[i].color =  glm::vec3(0);
        }

        return vertices;
    }

    //
    // OpenGL Helper Funtions
    // 

    vao createVertexArrayObject(std::vector<vertex> vertices)
    {
        // load vertex data
        GLuint vertexCount = vertices.size();

        // create vertex array object
        GLuint vertexArrayObject;
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);

        // create vertex buffer object
        GLuint vertexBufferObject;
        glGenBuffers(1, &vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

        // assign vertex attributes
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texcoord));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));

        // cleanup
        glBindVertexArray(0);

        return { vertexArrayObject, vertexBufferObject, vertexCount };
    }

    GLuint compileShader(GLuint type, const GLchar* shaderSource)
    {
        // create and compile shader
        GLuint shaderID = glCreateShader(type);
        glShaderSource(shaderID, 1, &shaderSource, 0);
        glCompileShader(shaderID);

        // check compile success
        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled != GL_TRUE)
        {
            GLint logLength;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> log(logLength);
            glGetShaderInfoLog(shaderID, logLength, &logLength, log.data());
            glDeleteShader(shaderID);
            std::cout << log.data() << std::endl;
            return 0;
        }

        return shaderID;
    }

    GLuint linkShaderProgram(GLuint vertexShader, GLuint fragmentShader)
    {
        if (!vertexShader) return 0;
        if (!fragmentShader) return 0;

        // link shader program
        GLuint shaderProgramID = glCreateProgram();
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, fragmentShader);
        glLinkProgram(shaderProgramID);

        // check link success
        GLint shaderProgramLinked = GL_TRUE;
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &shaderProgramLinked);
        if (shaderProgramLinked != GL_TRUE)
        {
            GLint logLength;
            glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> log(logLength);
            glGetProgramInfoLog(shaderProgramID, logLength, &logLength, log.data());
            glDeleteProgram(shaderProgramID);
            std::cout << log.data() << std::endl;
            return 0;
        }

        // set vertex attibute locations and texture units
        glUseProgram(shaderProgramID);
        glBindAttribLocation(shaderProgramID, 0, "vertexPosition");
        glBindAttribLocation(shaderProgramID, 1, "vertexTexcoord");
        glBindAttribLocation(shaderProgramID, 2, "vertexNormal");
        glBindAttribLocation(shaderProgramID, 3, "vertexColor");
        glLinkProgram(shaderProgramID);
        glUniform1i(glGetUniformLocation(shaderProgramID, "texture1"), 1);
        glUniform1i(glGetUniformLocation(shaderProgramID, "texture2"), 2);
        glUniform1i(glGetUniformLocation(shaderProgramID, "texture3"), 3);
        glUniform1i(glGetUniformLocation(shaderProgramID, "texture4"), 4);
        glUseProgram(0);

        // cleanup
        glDetachShader(shaderProgramID, vertexShader);
        glDetachShader(shaderProgramID, fragmentShader);

        return shaderProgramID;
    }

    GLuint loadShaderProgram(const char* vertShaderSourceFile, const char* fragShaderSourceFile)
    {
        // load shader source code
        const char* vertexShaderSource = loadShaderSource(vertShaderSourceFile);
        const char* fragmentShaderSource = loadShaderSource(fragShaderSourceFile);

        // compile shaders
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        // create shader program consisting of vertex and fragment shader
        GLuint ShaderProgram = linkShaderProgram(vertexShader, fragmentShader);

        // cleanup
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return ShaderProgram;
    }

    static void callbackFunctionError(int error, const char* message)
    {
        std::cerr << "GLFW Error: " << message << std::endl;
    }

    static void callbackFunctionKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    static void callbackFunctionMouseButton(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_1)
        {
            if (action == GLFW_PRESS)
                globalState.mouseDown = true;
            if (action == GLFW_RELEASE)
                globalState.mouseDown = false;
        }
    }

    static void callbackFunctionMousePos(GLFWwindow* window, double xpos, double ypos)
    {
        int width, height;
        glfwGetFramebufferSize(globalState.window, &width, &height);
        auto mousePos = glm::vec2(2.0f, -2.0f) * glm::vec2(xpos, ypos) / glm::vec2(width, height) - glm::vec2(1.0f, -1.0f);

        if (globalState.mouseCallback)
            globalState.mouseCallback(mousePos);

        if (globalState.mouseDown)
        {
            int width, height;
            glfwGetFramebufferSize(globalState.window, &width, &height);
            auto mouseDelta = 3.1416f * (mousePos - globalState.mousePos);

            globalState.cameraPos = glm::mat3(glm::rotate(glm::mat4(1.0f), -mouseDelta.x, glm::vec3(0, 1, 0))) * globalState.cameraPos;
            
            glm::vec3 cameraRight = glm::normalize(glm::vec3(-globalState.cameraPos.z, 0, globalState.cameraPos.x));
            auto newCamera = glm::mat3(glm::rotate(glm::mat4(1.0f), -mouseDelta.y, cameraRight)) * globalState.cameraPos;
            if (globalState.cameraPos.x * newCamera.x >= 0.0f &&
                globalState.cameraPos.z * newCamera.z >= 0.0f)
                globalState.cameraPos = newCamera;
        }

        globalState.mousePos = mousePos;
    }

    void callbackFunctionMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (globalState.scrollCallback)
            globalState.scrollCallback((float)yoffset, globalState.mousePos);
    }

    static void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        fprintf(stderr, "GL Debug Message: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
    }

    bool init(const char *WindowName)
    {
        glfwSetErrorCallback(callbackFunctionError);

        if (!glfwInit())
            return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        globalState.window = glfwCreateWindow(1024, 768, WindowName, NULL, NULL);
        if (!globalState.window)
        {
            glfwTerminate();
            return false;
        }
        
        glfwSetTime(0.0);

        glfwMakeContextCurrent(globalState.window);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);
        glfwSetKeyCallback(globalState.window, callbackFunctionKeyboard);
        glfwSetMouseButtonCallback(globalState.window, callbackFunctionMouseButton);
        glfwSetCursorPosCallback(globalState.window, callbackFunctionMousePos);
        glfwSetScrollCallback(globalState.window, callbackFunctionMouseScroll);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        ImGui_ImplGlfw_InitForOpenGL(globalState.window, true);
        ImGui_ImplOpenGL3_Init("#version 150");

        if (glDebugMessageCallback)
            glDebugMessageCallback(debugMessageCallback, 0);

        globalState.cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);

        return true;
    }

    void destroy()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(globalState.window);
        glfwTerminate();
    }

    void beginFrame()
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void endFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
        glfwSwapBuffers(globalState.window);
    }

    bool isRunning()
    {
        return !glfwWindowShouldClose(globalState.window);
    }

    glm::vec2 getWindowSize()
    {
        int width, height;
        glfwGetFramebufferSize(globalState.window, &width, &height);
        return glm::vec2(width, height);
    }

    glm::mat4 getCamera()
    {
        return glm::lookAt(globalState.cameraPos, glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    float getTime()
    {
        return (float)glfwGetTime();
    }
    
    void setMouseCallback(mouse_callback_func Func)
    {
        globalState.mouseCallback = Func;
    }

    void setScrollCallback(scroll_callback_func Func)
    {
        globalState.scrollCallback = Func;
    }
}
