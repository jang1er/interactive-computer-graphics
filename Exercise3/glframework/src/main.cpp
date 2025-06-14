
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <array>
#include <vector>
#include <stdio.h>

struct astro_object
{
    /** The distance to the planet systems center of mass. */
    float orbitDistance;
    /** The objects orbit duration. */
    float orbitPeriod;
    /** The objects equatorial and pole radius. */
    glm::vec2 size;
    /** The objects rotation axis. */
    float rotAxis;
    /** The duration of a rotation. */
    float rotPeriod;
};

struct planet_system
{
    /** The plane this system orbits around the sun. */
    float orbitPlane;
    /** The systems distance to the sun. */
    float orbitDistance;
    /** The systems orbit duration. */
    float orbitPeriod;
    /** The plane all of the systems objects orbit on. */
    float localSystemOrbitPlane;
    /** Holds a list of all objects in the system. */
    std::vector<astro_object> objects;
};

// ASTROLOGICAL CONSTANTS
const float distanceEarthSystem = 293.752f;
const float distancePlutoSystem = 578.754f;
const float distanceMoon = 30.134f;
const float distancePluto = -2.0f;
const float distanceCharon = 6.084f;

const glm::vec2 sizeSun{ 27.179f, 27.178f };
const glm::vec2 sizeEarth{ 10.0f, 9.97 };
const glm::vec2 sizeMoon{ 2.73, 2.72f };
const glm::vec2 sizePluto{ 1.87f, 1.84f };
const glm::vec2 sizeCharon{ 0.95f };

const float orbitPlaneEarth{ 0.0f };
const float orbitPlanePluto{ 17.16f };

const float orbitPlaneMoon{ 5.145f };
const float orbitPlaneCharon{ 119.591f };

const float rotAxisSun{ 7.25f };
const float rotAxisEarth{ 23.44f };
const float rotAxisMoon{ 6.68f };
const float rotAxisPluto{ 122.53f };
const float rotAxisCharon{ 0.0f };

const float rotPeriodSun = 25.38f;
const float rotPeriodEarth = 1.0f;
const float rotPeriodMoon = 27.321f;
const float rotPeriodPluto = 6.375f;
const float rotPeriodCharon = 6.387f;

const float orbitPeriodEarth = 365.256f;
const float orbitPeriodMoon = 27.321f;
const float orbitPeriodPlutoSystem = 65703.835f;
const float orbitPeriodPluto = 6.387f;
const float orbitPeriodCharon = 6.387f;

#include "glframework.h"

std::vector<glframework::vertex> createPlanetVertices()
{
    std::vector<glframework::vertex> vertices;

    const unsigned int polarSteps = 24;
    const unsigned int azimuthalSteps = 24;

    // add top cap
    float phiOld = 0.0f;
    float thetaOld = 0.0f;
    float theta = glm::pi<float>() * static_cast<float>(1) / static_cast<float>(polarSteps);
    for (unsigned int i = 0; i < azimuthalSteps; ++i) {
        float phi = glm::two_pi<float>() * static_cast<float>(i + 1) / static_cast<float>(azimuthalSteps);
        glm::vec3 p0{ 0.0f, 1.0f, 0.0f };
        glm::vec3 p1{ glm::sin(theta) * glm::cos(phiOld), glm::cos(theta), glm::sin(theta) * glm::sin(phiOld) };
        glm::vec3 p2{ glm::sin(theta) * glm::cos(phi), glm::cos(theta), glm::sin(theta) * glm::sin(phi) };

        vertices.push_back(glframework::vertex{ p0,
            glm::vec2(phi / glm::two_pi<float>(), 1.0f - thetaOld / glm::pi<float>()) });
        vertices.push_back(glframework::vertex{ p1,
            glm::vec2(phiOld / glm::two_pi<float>(), 1.0f - theta / glm::pi<float>()) });
        vertices.push_back(glframework::vertex{ p2,
            glm::vec2(phi / glm::two_pi<float>(), 1.0f - theta / glm::pi<float>()) });
        phiOld = phi;
    }
    // add stripes
    thetaOld = theta;
    for (unsigned int j = 1; j < polarSteps; ++j) {
        theta = glm::pi<float>() * static_cast<float>(j) / static_cast<float>(polarSteps);
        phiOld = 0.0f;
        for (unsigned int i = 0; i < azimuthalSteps; ++i) {
            float phi = glm::two_pi<float>() * static_cast<float>(i + 1) / static_cast<float>(azimuthalSteps);
            glm::vec3 p0{ glm::sin(thetaOld) * glm::cos(phiOld), glm::cos(thetaOld),
                         glm::sin(thetaOld) * glm::sin(phiOld) };
            glm::vec3 p1{ glm::sin(theta) * glm::cos(phiOld), glm::cos(theta), glm::sin(theta) * glm::sin(phiOld) };
            glm::vec3 p2{ glm::sin(theta) * glm::cos(phi), glm::cos(theta), glm::sin(theta) * glm::sin(phi) };
            glm::vec3 p3{ glm::sin(thetaOld) * glm::cos(phi), glm::cos(thetaOld),
                         glm::sin(thetaOld) * glm::sin(phi) };

            vertices.push_back(glframework::vertex{ p0,
                glm::vec2(phiOld / glm::two_pi<float>(), 1.0f - thetaOld / glm::pi<float>()) });
            vertices.push_back(glframework::vertex{ p1,
                glm::vec2(phiOld / glm::two_pi<float>(), 1.0f - theta / glm::pi<float>()) });
            vertices.push_back(glframework::vertex{ p2,
                glm::vec2(phi / glm::two_pi<float>(), 1.0f - theta / glm::pi<float>()) });

            vertices.push_back(glframework::vertex{ p0,
                glm::vec2(phiOld / glm::two_pi<float>(), 1.0f - thetaOld / glm::pi<float>()) });
            vertices.push_back(glframework::vertex{ p2,
                glm::vec2(phi / glm::two_pi<float>(), 1.0f - theta / glm::pi<float>()) });
            vertices.push_back(glframework::vertex{ p3,
                glm::vec2(phi / glm::two_pi<float>(), 1.0f - thetaOld / glm::pi<float>()) });

            phiOld = phi;
        }
        thetaOld = theta;
    }

    // add bottom cap
    phiOld = 0.0f;
    for (unsigned int i = 0; i < azimuthalSteps; ++i) {
        float phi = glm::two_pi<float>() * static_cast<float>(i + 1) / static_cast<float>(azimuthalSteps);
        glm::vec3 p0{ 0.0f, -1.0f, 0.0f };
        glm::vec3 p1{ glm::sin(theta) * glm::cos(phi), glm::cos(theta), glm::sin(theta) * glm::sin(phi) };
        glm::vec3 p2{ glm::sin(theta) * glm::cos(phiOld), glm::cos(theta), glm::sin(theta) * glm::sin(phiOld) };
        
        vertices.push_back(glframework::vertex{ p0, 1.0f - glm::vec2(phi / glm::two_pi<float>(), 1.0f) });
        vertices.push_back(glframework::vertex{ p1,
            glm::vec2(phiOld / glm::two_pi<float>(), 1.0f - theta / glm::pi<float>()) });
        vertices.push_back(glframework::vertex{ p2,
            glm::vec2(phi / glm::two_pi<float>(), 1.0f - theta / glm::pi<float>()) });
        phiOld = phi;
    }

    return vertices;
}

int main()
{
    if (!glframework::init("Interaktive Computergrafik 3"))
        return 1;

    // load shader
    GLuint shaderProgram = glframework::loadShaderProgram("shaders/planet.vert", "shaders/planet.frag");

    // get uniform locations
    GLuint MVPUniformLocation = glGetUniformLocation(shaderProgram, "MVP");

    // create the planet mesh
    auto planetVertices = createPlanetVertices();
    auto planetVAO = glframework::createVertexArrayObject(planetVertices);

    // load textures
    std::vector<glframework::texture> textures{
        glframework::loadTexture("res/planets/sun.jpg"),
        glframework::loadTexture("res/planets/earth.jpg"),
        glframework::loadTexture("res/planets/moon.jpg"),
        glframework::loadTexture("res/planets/pluto.jpg"),
        glframework::loadTexture("res/planets/charon.jpg") };

    // create world matrices
    std::vector<glm::mat4> worldMatrices{ 
        glm::mat4{ 1.0f },
        glm::mat4{ 1.0f },
        glm::mat4{ 1.0f },
        glm::mat4{ 1.0f },
        glm::mat4{ 1.0f } };

    // create astrological objects and planet ssytems
    const astro_object sun{ 0.0f, 0.0f, sizeSun, rotAxisSun, rotPeriodSun };
    const astro_object earth{ 0.0f, 0.0f, sizeEarth, rotAxisEarth, rotPeriodEarth };
    const astro_object moon{ distanceMoon, orbitPeriodMoon, sizeMoon, rotAxisMoon, rotPeriodMoon };
    const astro_object pluto{ 0.0f, 0.0f, sizePluto, rotAxisPluto, rotPeriodPluto };
    const astro_object charon{ distanceCharon, orbitPeriodCharon, sizeCharon, rotAxisCharon, rotPeriodCharon };
    const planet_system earthMoon{ orbitPlaneEarth, distanceEarthSystem, orbitPeriodEarth, orbitPlaneMoon, {earth, moon} };
    const planet_system plutoCharon{ orbitPlanePluto, distancePlutoSystem, orbitPeriodPlutoSystem, orbitPlaneCharon, { pluto, charon } };
    const std::vector<planet_system> planetSystems{ earthMoon, plutoCharon };
    
    // system world matrix
    // TODO (b): Modify earthSystemWorldMatrix to adjust camera system. 
    auto earthSystemWorldMatrix = glm::mat4{ 1.0f };

    // set rendering parameters
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    float deltatime = 0;
    float secsPerDay = 5;
    float start = glframework::getTime();

    // main rendering loop
    while (glframework::isRunning())
    {
        glframework::beginFrame();

        // update rendered image size
        auto screen = glframework::getWindowSize();
        glViewport(0, 0, screen.x, screen.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO (c): Modify for time adjustments.
<<<<<<< HEAD
        float rotTime = (glframework::getTime() - start) / 5.0f;
=======
        float rotTime = (glframework::getTime() - start) / 5.0f * 1000;
>>>>>>> refs/remotes/origin/dev

        // TODO (a): Create matrices for sun and other objects (worldMatrices[0] ... worldMatrices[4]).
        glm::mat4 identity = glm::mat4(1.0);

        float rot = rotTime * glm::two_pi<float>();

        // sun
        worldMatrices[0] =  identity * glm::scale(glm::vec3(sun.size, sun.size.x)); 
        worldMatrices[0] *= glm::rotate(rot / sun.rotPeriod, glm::vec3(0.0f, 1.0f, 0.0f));
        
        for(int i = 0; i < 4; ++i){
            int index = i % 2;
            planet_system system = i > 1 ? plutoCharon : earthMoon;
            int j = i+1;
            astro_object obj = system.objects[index];
            
            glm::mat4 model = identity;
            // global transformations
            model = glm::rotate(model, rot / system.orbitPeriod, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(system.orbitDistance, system.orbitPlane, 0.0f)); // translate system orbit distance and orbitPlane)

            // local transformations
            if(obj.orbitPeriod > 0.0f)model = glm::rotate(model, rot / obj.orbitPeriod, glm::vec3(0.0f, 1.0f, 0.0f));// rotate orbit
            model = glm::translate(model, glm::vec3(obj.orbitDistance, system.localSystemOrbitPlane, 0.0f));// translate orbit distance and localSystemPlane
            model = glm::rotate(model, glm::radians(obj.rotAxis), glm::vec3(1.0f, 0.0f, 0.0f));// rotate to rotAxis
            model = glm::rotate(model,  rot / obj.rotPeriod, glm::vec3(0.0f, 1.0f, 0.0f));// rotate for rotation
            model = glm::scale(model, glm::vec3(obj.size, obj.size.x)); // scale to size
            worldMatrices[j] = model;
        }


        //std::cout << glm::to_string(worldMatrices[0]) << std::endl;


        // set matrices 
        auto v = glframework::getCameraView();
        auto p = glframework::getCameraPerspective();
        
        glm::mat4 model = identity;
        model = glm::rotate(model, rot / earthMoon.orbitPeriod, glm::vec3(0.0f, 1.0f, 0.0f));
        earthSystemWorldMatrix = glm::translate(model, glm::vec3(earthMoon.orbitDistance, earthMoon.orbitPlane, 0.0f));


        // (The scaling is only for the world to be smaller and should be left at 100)
        auto vp = p * v * glm::inverse(glm::scale(earthSystemWorldMatrix, glm::vec3(100.0)));

        // set shader for rendering
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE1);
        glBindVertexArray(planetVAO.id);

        // draw planets
        for (int i = 0; i < 5; ++i)
        {
            glm::mat4 mvpMatrix = vp * worldMatrices[i];
            glUniformMatrix4fv(MVPUniformLocation, 1, GL_FALSE, (const float *)glm::value_ptr(mvpMatrix));
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
            glDrawArrays(GL_TRIANGLES, 0, planetVAO.vertexCount);
        }
        
        glframework::endFrame();
    }

    glframework::destroy();

    return 0;
}
