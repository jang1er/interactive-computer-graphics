
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <array>
#include <vector>
#include <stdio.h>

struct vertex
{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    glm::vec3 color;
};

#include "glframework.h"

namespace glframework
{
    struct vao
    {
        GLuint id;
        GLuint vbo;
        GLuint vertexCount;
    };

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
}

// calulate the normal of triangle face defined by given vertex positions in counter clockwise order
glm::vec3 calculateNormal(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c)
{
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 n = glm::cross(ab, ac);
    return glm::normalize(n);
}

std::vector<vertex> createCubeVertices()
{
    std::vector<vertex> vertices;
    
    // TODO: create a cube here instead of a triangle and apply colors and correct normals
    
   
    // Front Face
    vertices.push_back(vertex{ glm::vec3(-1,-1, 1), glm::vec2(), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0) }); //position, texture uv, normal, color 
    vertices.push_back(vertex{ glm::vec3(1,-1, 1), glm::vec2(), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0) });
    vertices.push_back(vertex{ glm::vec3(-1,1, 1), glm::vec2(), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0) });
    vertices.push_back(vertex{ glm::vec3(1,-1, 1), glm::vec2(), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0) });
    vertices.push_back(vertex{ glm::vec3(1,1, 1), glm::vec2(), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0) });
    vertices.push_back(vertex{ glm::vec3(-1,1, 1), glm::vec2(), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0) });

    //Back Face
    vertices.push_back(vertex{ glm::vec3(-1,-1, -1), glm::vec2(), glm::vec3(0, 0, -1), glm::vec3(0, 1, 1) });
    vertices.push_back(vertex{ glm::vec3(-1,1, -1), glm::vec2(), glm::vec3(0, 0, -1), glm::vec3(0, 1, 1) });
    vertices.push_back(vertex{ glm::vec3(1,-1, -1), glm::vec2(), glm::vec3(0, 0, -1), glm::vec3(0, 1, 1) });
    vertices.push_back(vertex{ glm::vec3(1,-1, -1), glm::vec2(), glm::vec3(0, 0, -1), glm::vec3(0, 1, 1) });
    vertices.push_back(vertex{ glm::vec3(-1,1, -1), glm::vec2(), glm::vec3(0, 0, -1), glm::vec3(0, 1, 1) });
    vertices.push_back(vertex{ glm::vec3(1,1, -1), glm::vec2(), glm::vec3(0, 0, -1), glm::vec3(0, 1, 1) });

    vertices.push_back(vertex{ glm::vec3(-1, -1,-1), glm::vec2(), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(1, -1,-1), glm::vec2(), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(-1, -1,1), glm::vec2(), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(1, -1,-1), glm::vec2(), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(1, -1,1), glm::vec2(), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(-1, -1,1), glm::vec2(), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0) });

    vertices.push_back(vertex{ glm::vec3(-1,1, -1), glm::vec2(), glm::vec3(0, 1, 0), glm::vec3(1, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(-1, 1,1), glm::vec2(), glm::vec3(0, 1, 0), glm::vec3(1, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(1, 1,-1), glm::vec2(), glm::vec3(0, 1, 0), glm::vec3(1, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(1, 1,-1), glm::vec2(), glm::vec3(0, 1, 0), glm::vec3(1, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(-1, 1,1), glm::vec2(), glm::vec3(0, 1, 0), glm::vec3(1, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(1, 1,1), glm::vec2(), glm::vec3(0, 1, 0), glm::vec3(1, 0, 1) });



    vertices.push_back(vertex{ glm::vec3(1, -1,-1), glm::vec2(), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(1, 1,-1), glm::vec2(), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(1, -1,1), glm::vec2(), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(1, 1,-1), glm::vec2(), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(1, 1,1), glm::vec2(), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) });
    vertices.push_back(vertex{ glm::vec3(1, -1,1), glm::vec2(), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) });

    vertices.push_back(vertex{ glm::vec3(-1,-1, -1), glm::vec2(), glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(-1, -1,1), glm::vec2(), glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(-1, 1,-1), glm::vec2(), glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(-1, 1,-1), glm::vec2(), glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(-1, -1,1), glm::vec2(), glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0) });
    vertices.push_back(vertex{ glm::vec3(-1, 1,1), glm::vec2(), glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0) });



    
    return vertices;
}

using tetrahedron = std::array<vertex, 4>;

// calculate linear interpolation of two vertices
vertex vertexLerp(vertex a, vertex b, float t)
{
    vertex c{};
    c.position = (a.position + b.position) * t;
    c.texcoord = (a.texcoord + b.texcoord) * t;
    c.normal = (a.normal+ b.normal) * t;
    c.color = (a.color + b.color) * t;
    return c;
}

std::vector<tetrahedron> splitFractalTetrahedron(std::vector<tetrahedron> tetrahedra, int depth = 0)
{
    std::vector<tetrahedron> result;

    for (const auto& th : tetrahedra)
    {
        std::array<tetrahedron, 4> splitTetrahedra{};

        // TODO: Subdivide tetrahedron to create 4 new ones.
        // splitTetrahedra[0][0] = ;
        // Each vertex is connected to each other vertex. The subdivision is done by computing the midpoint between all indices:
        for (int i = 0; i < 4; ++i){
            for (int j = 0; j < 4; ++j){
                splitTetrahedra[i][j] = vertexLerp(th[i], th[j], 0.5f);
            }
        }


        result.insert(result.end(), splitTetrahedra.cbegin(), splitTetrahedra.cend());
    }

    if (depth < 5)
        return splitFractalTetrahedron(result, depth + 1);

    return result;
}

std::vector<vertex> createFractalTetrahedronVertices()
{
    tetrahedron initialTetrahedron{};
    
    // TODO: create a tetrahedron containing 4 vertices.
    initialTetrahedron[0] = { glm::vec3(-1,-1,-1), glm::vec2(), glm::vec3(), glm::vec3(1,1,1) };
    initialTetrahedron[1] = { glm::vec3(0,1,0), glm::vec2(), glm::vec3(), glm::vec3(0,0,1) };
    initialTetrahedron[2] = { glm::vec3(1,-1,-1), glm::vec2(), glm::vec3(), glm::vec3(1,0,0) };
    initialTetrahedron[3] = { glm::vec3(0,-1,1), glm::vec2(), glm::vec3(), glm::vec3(0,1,0) };

    //initialTetrahedron[0] = { glm::vec3(1,1,1), glm::vec2(), glm::vec3(), glm::vec3(1,1,1) };
    //initialTetrahedron[1] = { glm::vec3(-1,-1,1), glm::vec2(), glm::vec3(), glm::vec3(0,0,1) };
    //initialTetrahedron[2] = { glm::vec3(1,-1,-1), glm::vec2(), glm::vec3(), glm::vec3(1,0,0) };
    //initialTetrahedron[3] = { glm::vec3(-1,1,-1), glm::vec2(), glm::vec3(), glm::vec3(0,1,0) };

    // ...

    std::vector<tetrahedron> FractalTetrahedra = splitFractalTetrahedron(std::vector<tetrahedron>{ initialTetrahedron });

    std::vector<vertex> vertices;
    // add all the tetrahedron faces to the vertex list as triangles
    for (const auto& tetrahedron : FractalTetrahedra)
    {
        // TODO: calculate correct normals.
        glm::vec3 normal = glm::vec3();
        vertices.push_back(tetrahedron[0]);
        vertices.back().normal = normal;
        vertices.push_back(tetrahedron[1]);
        vertices.back().normal = normal;
        vertices.push_back(tetrahedron[2]);
        vertices.back().normal = normal;

        normal = glm::vec3();
        vertices.push_back(tetrahedron[0]);
        vertices.back().normal = normal;
        vertices.push_back(tetrahedron[2]);
        vertices.back().normal = normal;
        vertices.push_back(tetrahedron[3]);
        vertices.back().normal = normal;

        normal = glm::vec3();
        vertices.push_back(tetrahedron[0]);
        vertices.back().normal = normal;
        vertices.push_back(tetrahedron[3]);
        vertices.back().normal = normal;
        vertices.push_back(tetrahedron[1]);
        vertices.back().normal = normal;

        normal = glm::vec3();
        vertices.push_back(tetrahedron[1]);
        vertices.back().normal = normal;
        vertices.push_back(tetrahedron[3]);
        vertices.back().normal = normal;
        vertices.push_back(tetrahedron[2]);
        vertices.back().normal = normal;
    }

    return vertices;
}

int main()
{
    if (!glframework::init("Interaktive Computergrafik 1"))
        return 1;

    // load shader
    GLuint shaderProgram = glframework::loadShaderProgram("shaders/default.vert", "shaders/light.frag");
    GLint mvpLocation = glGetUniformLocation(shaderProgram, "MVP");

    // create the cube mesh
    auto cubeVertices = createCubeVertices();
    auto cubaVAO = glframework::createVertexArrayObject(cubeVertices);

    // create the tetrahedron mesh
    auto tetrahedronVertices = createFractalTetrahedronVertices();
    auto tetrahedronVAO = glframework::createVertexArrayObject(tetrahedronVertices);

    // set rendering parameters
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    int drawVAO = 0;
    
    // main rendering loop
    while (glframework::isRunning())
    {
        glframework::beginFrame();

        // draw user interface
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Always);
        ImGui::Begin("Rendering Parameters");
        ImGui::RadioButton("Draw Cube", &drawVAO, 0);
        ImGui::RadioButton("Draw Tetrahedron", &drawVAO, 1);
        ImGui::End();

        // update rendered image size
        int width, height;
        glframework::getWindowSize(&width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set shader for rendering
        glUseProgram(shaderProgram);        
        
        // calculate and set model view projection matrix
        glm::mat4 m = glm::mat4(1.0f);
        glm::mat4 v = glframework::getCamera();
        glm::mat4 p = glm::perspective(glm::radians(30.0f), (float)width / (float)height, 0.1f, 10.0f);
        glm::mat4 mvp = p * v * m;
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
        
        // draw the selected vertex array object
        if (drawVAO == 0)
        {
            // draw cube
            glBindVertexArray(cubaVAO.id);
            glDrawArrays(GL_TRIANGLES, 0, 36); // TODO: Set the correct number of vertices to be rendered
        }
        else if (drawVAO == 1)
        {
            // draw fractal tetrahedron
            glBindVertexArray(tetrahedronVAO.id);
            glDrawArrays(GL_TRIANGLES, 0, tetrahedronVAO.vertexCount); // TODO: Set the correct number of vertices to be rendered
        }

        glframework::endFrame();
    }

    glframework::destroy();
    return 0;
}
