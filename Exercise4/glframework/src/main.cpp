
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <vector>
#include <stdio.h>

#include "glframework.h"

using vertex = glframework::vertex;

struct free_camera
{
    glm::vec3 position;

    void update(float delta)
    {
        if (glframework::getKeyDown('W'))
            this->position += delta * glm::vec3(0.0f, 0.0f, -1.0f);
        if (glframework::getKeyDown('A'))
            this->position += delta * glm::vec3(-1.0f, 0.0f, 0.0f);
        if (glframework::getKeyDown('S'))
            this->position += delta * glm::vec3(0.0f, 0.0f, 1.0f);
        if (glframework::getKeyDown('D'))
            this->position += delta * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::mat4 getViewProjection()
    {
        glm::vec2 windowSize = glframework::getWindowSize();
        auto p = glm::perspective(glm::radians(60.0f), (float)windowSize.x / (float)windowSize.y, 1.0f, 5.0f);
        auto v = glm::translate(glm::mat4(1.0f), -position);
        return p * v;
    }
    
    std::vector<vertex> getFrustumVertices()
    {
        auto inverseVP = glm::inverse(getViewProjection());

        std::vector<vertex> vertices;

        auto v000 = inverseVP[3] - inverseVP[0] - inverseVP[1] - inverseVP[2];
        auto v001 = inverseVP[3] + inverseVP[0] - inverseVP[1] - inverseVP[2];
        auto v010 = inverseVP[3] - inverseVP[0] + inverseVP[1] - inverseVP[2];
        auto v011 = inverseVP[3] + inverseVP[0] + inverseVP[1] - inverseVP[2];
        auto v100 = inverseVP[3] - inverseVP[0] - inverseVP[1] + inverseVP[2];
        auto v101 = inverseVP[3] + inverseVP[0] - inverseVP[1] + inverseVP[2];
        auto v110 = inverseVP[3] - inverseVP[0] + inverseVP[1] + inverseVP[2];
        auto v111 = inverseVP[3] + inverseVP[0] + inverseVP[1] + inverseVP[2];

        v000 /= v000.w;
        v001 /= v001.w;
        v010 /= v010.w;
        v011 /= v011.w;
        v100 /= v100.w;
        v101 /= v101.w;
        v110 /= v110.w;
        v111 /= v111.w;

        vertices.push_back(vertex{ v000, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v001, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v001, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v011, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v011, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v010, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v010, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v000, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });

        vertices.push_back(vertex{ v100, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v101, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v101, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v111, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v111, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v110, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v110, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v100, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });

        vertices.push_back(vertex{ v000, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v100, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v001, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v101, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v010, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v110, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v011, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });
        vertices.push_back(vertex{ v111, glm::vec2(), glm::vec3(), glm::vec3(1.0f) });

        return vertices;
    }
};

std::vector<vertex> createCubeVertices()
{
    std::vector<vertex> vertices;

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
     
    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });

    return vertices;
}

// Helper: compute signed distance from vertex to plane
float distanceToPlane(const glm::vec4& plane, const glm::vec3& point)
{
    return glm::dot(glm::vec3(plane), point) + plane.w;
}

// Helper: linearly interpolate two vertices
vertex interpolate(const vertex& a, const vertex& b, float t)
{
    return {
        glm::mix(a.position, b.position, t),
        glm::mix(a.texcoord, b.texcoord, t),
        glm::mix(a.normal, b.normal, t),
        glm::mix(a.color, b.color, t)
    };
}


std::vector<vertex> clipPolygonAgainstPlane(const std::vector<vertex>& polygon, const glm::vec4 plane){
    std::vector<vertex> output;

    for (size_t i = 0; i < polygon.size(); ++i)
    {
        const vertex& curr = polygon[i];
        const vertex& prev = polygon[(i + polygon.size() - 1) % polygon.size()];

        float distCurr = distanceToPlane(plane, curr.position);
        float distPrev = distanceToPlane(plane, prev.position);

        bool insideCurr = distCurr >= 0.0f;
        bool insidePrev = distPrev >= 0.0f;

        if (insideCurr && insidePrev)
        {
            output.push_back(curr);
        }
        else if (!insidePrev && insideCurr)
        {
            float t = distPrev / (distPrev - distCurr);
            output.push_back(interpolate(prev, curr, t));
            output.push_back(curr);
        }
        else if (insidePrev && !insideCurr)
        {
            float t = distPrev / (distPrev - distCurr);
            output.push_back(interpolate(prev, curr, t));
        }
    }

    return output;
}

std::vector<vertex> clipVertices(glm::mat4 mvp, std::vector<vertex> vertices)
{
    // TODO (a): Calculate the 6 view frustum planes from the mvp.
    std::array<glm::vec4, 6> frustumPlanes;

    // Extract planes
    glm::mat4 mvpT = glm::transpose(mvp);

    frustumPlanes[0] = mvpT[3] + mvpT[0]; // Left
    frustumPlanes[1] = mvpT[3] - mvpT[0]; // Right
    frustumPlanes[2] = mvpT[3] + mvpT[1]; // Bottom
    frustumPlanes[3] = mvpT[3] - mvpT[1]; // Top
    frustumPlanes[4] = mvpT[3] + mvpT[2]; // Near
    frustumPlanes[5] = mvpT[3] - mvpT[2]; // Far

    // normalize the planes
    for (auto& plane : frustumPlanes)
        plane /= glm::length(glm::vec3(plane));

    // TODO (b): Clip all triangles in vertices against the frustum. Save clipped vertices in clippedVertices.
    std::vector<vertex> clippedVertices;

    // go through all vertices (as a triangle)
    for (size_t i = 0; i < vertices.size(); i += 3)
    {
        // construct polygon (triangle)
        std::vector<vertex> poly = {
            vertices[i],
            vertices[i + 1],
            vertices[i + 2]
        };

        // clip against all frustum planes
        for (int p = 0; p < 6; ++p)
        {
            poly = clipPolygonAgainstPlane(poly, frustumPlanes[p]);
            if (poly.empty()) // no vertices left -> completly clipped
                break;
        }

        // Re-triangulate clipped polygon (fan method)
        for (size_t j = 1; j + 1 < poly.size(); ++j)
        {
            clippedVertices.push_back(poly[0]);
            clippedVertices.push_back(poly[j]);
            clippedVertices.push_back(poly[j + 1]);
        }
    }

    return clippedVertices;
}

int main()
{
    if (!glframework::init("Interaktive Computergrafik 4"))
        return 1;

    // place free moving camera
    bool useFreeCam = false;
    free_camera freeCam{ glm::vec3(0.0f, 0.0f, 3.0f) };

    // load shader
    GLuint shaderProgram = glframework::loadShaderProgram("shaders/simple.vert", "shaders/simple.frag");

    // get uniform locations
    GLuint MVPUniformLocation = glGetUniformLocation(shaderProgram, "MVP");

    // create the cube mesh
    auto cubeVertices = createCubeVertices();
    auto cubeVAO = glframework::createVertexArrayObject(cubeVertices);

    // create the frustum mesh
    auto frustumVAO = glframework::createVertexArrayObject(std::vector<vertex>(24));

    // set rendering parameters
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // main rendering loop
    while (glframework::isRunning())
    {
        glframework::beginFrame();

        // draw user interface
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Always);
        ImGui::Begin("Rendering Parameters");
        ImGui::Checkbox("free camera view", &useFreeCam);
        ImGui::End();

        // update rendered image size
        auto screen = glframework::getWindowSize();
        glViewport(0, 0, screen.x, screen.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // get model-view-projection matrix
        auto m = glm::mat4(1.0f);
        auto vp = glframework::getViewProjection();
        auto mvp = vp * m;

        // update frustum vertices
        freeCam.update(0.1f);
        auto freeCamMVP = freeCam.getViewProjection();
        if (useFreeCam) mvp = freeCamMVP;
        auto frustumVertices = freeCam.getFrustumVertices();
        glBindBuffer(GL_ARRAY_BUFFER, frustumVAO.vbo);
        glBufferData(GL_ARRAY_BUFFER, frustumVertices.size() * sizeof(vertex), frustumVertices.data(), GL_STREAM_DRAW);
        frustumVAO.vertexCount = frustumVertices.size();

        // update cube vertices
        auto clippedCubeVertices = clipVertices(freeCamMVP, cubeVertices);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAO.vbo);
        glBufferData(GL_ARRAY_BUFFER, clippedCubeVertices.size() * sizeof(vertex), clippedCubeVertices.data(), GL_STREAM_DRAW);
        cubeVAO.vertexCount = clippedCubeVertices.size();

        // set shader for rendering
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(MVPUniformLocation, 1, GL_FALSE, (const float*)glm::value_ptr(mvp));

        // draw cube
        glBindVertexArray(cubeVAO.id);
        glDrawArrays(GL_TRIANGLES, 0, cubeVAO.vertexCount);

        // draw frustum
        glBindVertexArray(frustumVAO.id);
        glDrawArrays(GL_LINES, 0, frustumVAO.vertexCount);

        glframework::endFrame();
    }

    glframework::destroy();

    return 0;
}
