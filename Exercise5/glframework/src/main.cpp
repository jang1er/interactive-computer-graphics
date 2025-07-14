
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <vector>
#include <stdio.h>

#include "glframework.h"
#include <algorithm>

using vertex = glframework::vertex;

struct software_framebuffer
{
    glm::ivec2 size;
    std::vector<glm::vec3> colorBuffer;
    std::vector<float> depthBuffer;

    void resize(glm::vec2 framebufferSize)
    {
        size = framebufferSize;
        colorBuffer.resize(size.x * size.y * sizeof(glm::vec3));
        depthBuffer.resize(size.x * size.y * sizeof(float));
    }

    void clear()
    {
        std::fill(colorBuffer.begin(), colorBuffer.end(), glm::vec3(0.0f));
        std::fill(depthBuffer.begin(), depthBuffer.end(), 1.0f);
    }

    void setColorValue(int x, int y, glm::vec3 color)
    {
        colorBuffer[x + y * size.x] = color;
    }

    void setDepthValue(int x, int y, float depth)
    {
        depthBuffer[x + y * size.x] = depth;
    }

    float getDepthValue(int x, int y)
    {
        return depthBuffer[x + y * size.x];
    }
};

std::vector<vertex> createCubeVertices()
{
    std::vector<vertex> vertices;

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 1.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(0.0f, 0.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });

    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 1.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back(vertex{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(), glm::vec3(), glm::vec3(1.0f, 1.0f, 0.0f) });

    return vertices;
}

void rasterize(software_framebuffer &framebuffer, glm::mat4 mvp, std::vector<vertex> vertices)
{
    for (int triIdx = 0; triIdx < vertices.size();) {
        std::array<vertex, 3> tri3d{ vertices[triIdx], vertices[triIdx + 1], vertices[triIdx + 2] };
        std::array<glm::vec3, 3> screenCoords;
        // TODO: Get screen coordinates for the triangle using the MVP matrix (mvp) and the framebuffer size (framebuffer.size).
        for(int i = 0; i < screenCoords.size(); ++i){
            std::array<glm::vec4, 3> clipCoords; // temporary storage
            clipCoords[i] = mvp * glm::vec4(tri3d[i].position, 1.0f);
            clipCoords[i] /= clipCoords[i].w; // transform to NDC

            // interpolate to screen coordinates
            screenCoords[i].x = (clipCoords[i].x * 0.5f + 0.5f) * framebuffer.size.x; // interpolate x (0 to width)
            screenCoords[i].y = ((clipCoords[i].y * 0.5f + 0.5f)) * framebuffer.size.y; // interpolate and flip y (0 to height)
            screenCoords[i].z = clipCoords[i].z; // preserve z-value for depth test
        }
        // TODO: rasterize triangle using the scanline algorithm
        struct screen_vertex {
            glm::vec3 pos;
            glm::vec3 color;
        };
        std::array<screen_vertex, 3> sv{
            screen_vertex{ screenCoords[0], tri3d[0].color },
            screen_vertex{ screenCoords[1], tri3d[1].color },
            screen_vertex{ screenCoords[2], tri3d[2].color }
        };
        // sort the vertices based on y-coords ()
        std::sort(sv.begin(), sv.end(), [](const screen_vertex &a, const screen_vertex &b) {
            return a.pos.y < b.pos.y;
        });

        // for easier access
        auto &v0 = sv[0];
        auto &v1 = sv[1];
        auto &v2 = sv[2];

        // interpolate two vertices (position and color)
        auto interpolate = [](float y, const screen_vertex &a, const screen_vertex &b) -> screen_vertex {
            float t = (y - a.pos.y) / (b.pos.y - a.pos.y);
            glm::vec3 pos = glm::mix(a.pos, b.pos, t);
            glm::vec3 color = glm::mix(a.color, b.color, t);
            return { pos, color };
        };

        // rasterize the scanline and perform interpolation and depth test
        auto raster_scanline = [&](float y, screen_vertex left, screen_vertex right) {
            if (left.pos.x > right.pos.x) std::swap(left, right);
            int xStart = std::max(0, int(std::floor(left.pos.x)));
            int xEnd = std::min(framebuffer.size.x - 1, int(std::ceil(right.pos.x)));

            for (int x = xStart; x <= xEnd; ++x) {
                float t = (x - left.pos.x) / (right.pos.x - left.pos.x);
                float z = glm::mix(left.pos.z, right.pos.z, t);
                glm::vec3 color = glm::mix(left.color, right.color, t);

                if (z < framebuffer.getDepthValue(x, int(y))) {
                    framebuffer.setDepthValue(x, int(y), z);
                    framebuffer.setColorValue(x, int(y), color);
                }
            }
        };

        // rasterize the lower part (v0 -> v1)
        if (v1.pos.y != v0.pos.y) {
            int yStart = std::max(0, int(std::ceil(v0.pos.y)));
            int yEnd = std::min(framebuffer.size.y - 1, int(std::floor(v1.pos.y)));
            for (int y = yStart; y <= yEnd; ++y) {
                screen_vertex left = interpolate(y, v0, v1);
                screen_vertex right = interpolate(y, v0, v2);
                raster_scanline(y, left, right);
            }
        }

        // rasterize the upper part (v1 -> v2)
        if (v2.pos.y != v1.pos.y) {
            int yStart = std::max(0, int(std::ceil(v1.pos.y)));
            int yEnd = std::min(framebuffer.size.y - 1, int(std::floor(v2.pos.y)));
            for (int y = yStart; y <= yEnd; ++y) {
                screen_vertex left = interpolate(y, v1, v2);
                screen_vertex right = interpolate(y, v0, v2);
                raster_scanline(y, left, right);
            }
        }
        triIdx += 3;
    }

    bool drawTestImage = false;

    if (drawTestImage)
    {// draws a test image

        glm::uvec2 center = glm::vec2(framebuffer.size) / glm::vec2(2);
        for (unsigned int x = 0; x < framebuffer.size.x; x++) {
            for (unsigned int y = 0; y < framebuffer.size.y; y++) {
                float length = glm::distance(glm::vec2(x, y), glm::vec2(center));

                framebuffer.setColorValue(x, y,
                    glm::clamp(powf(length / 700.0f, 0.6f), 0.0f, 1.0f) * sinf(0.93f * length)
                    * cosf(5.3f * x) * glm::vec3(1.0f));
                if (length < 100.0f) {
                    glm::ivec2 delta = glm::ivec2(x, y) - glm::ivec2(center);
                    framebuffer.setColorValue(x, y, glm::vec3(delta.x > 0 ? 1.0f : 0.0f, delta.y > 0 ? 1.0f : 0.0f, 0.0f));
                }
            }
        }
    }
}

int main()
{
    if (!glframework::init("Interaktive Computergrafik 5"))
        return 1;

    // load shader
    GLuint shaderProgram = glframework::loadShaderProgram("shaders/simple.vert", "shaders/simple.frag");

    // get uniform locations
    GLuint MVPUniformLocation = glGetUniformLocation(shaderProgram, "MVP");

    // create the cube mesh
    auto cubeVertices = createCubeVertices();
    auto cubeVAO = glframework::createVertexArrayObject(cubeVertices);

    // set rendering parameters
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // software renderer
    software_framebuffer softwareFramebuffer;
    
    GLuint framebuffer = 0;
    GLuint framebufferTexture = 0;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGBA8, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    bool useSoftwareRasterizer = false;

    // main rendering loop
    while (glframework::isRunning())
    {
        glframework::beginFrame();

        // draw user interface
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Always);
        ImGui::Begin("Rendering Parameters");
        ImGui::Checkbox("Software Rasterizer", &useSoftwareRasterizer);
        ImGui::End();

        // get model-view-projection matrix
        auto m = glm::mat4(1.0f);
        auto vp = glframework::getViewProjection();
        auto mvp = vp * m;

        // set shader for rendering
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(MVPUniformLocation, 1, GL_FALSE, (const float*)glm::value_ptr(mvp));

        // update rendered image size
        auto screen = glframework::getWindowSize();
        
        // draw cube
        if (useSoftwareRasterizer)
        {
            softwareFramebuffer.resize(screen);
            softwareFramebuffer.clear();

            rasterize(softwareFramebuffer, mvp, cubeVertices);

            glBindTexture(GL_TEXTURE_2D, framebufferTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, screen.x, screen.y, 0, GL_RGB, GL_FLOAT, softwareFramebuffer.colorBuffer.data());
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
            glBlitFramebuffer(0, 0, screen.x, screen.y,
                              0, 0, screen.x, screen.y,
                              GL_COLOR_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else
        {
            glViewport(0, 0, screen.x, screen.y);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(cubeVAO.id);
            glDrawArrays(GL_TRIANGLES, 0, cubeVAO.vertexCount);
        }

        glframework::endFrame();
    }

    glframework::destroy();

    return 0;
}
