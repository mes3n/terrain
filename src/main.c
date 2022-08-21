#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "math/matrix.h"
#include "rendering/rendering.h"

#include "world/generation.h"
#include "world/noise.h"


int main (void) 
{   

    Camera camera;
    initCamera(&camera);

    GLFWwindow* window = createWindow("OpenGL", &camera);
    if (window == NULL)
        return -1;

    GLuint shader = loadShaders("shaders/basic.vert", "shaders/basic.frag");


    int terrainSize;
    GLuint terrain = generateTerrain(0.0f, -10.0f, 0.0f, 6, 5, 64.0f, &terrainSize);
    
    glUseProgram(shader);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glLineWidth(5.0f);

    float matrix3d[16];

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        getView(window, camera, matrix3d);
        setMat4(shader, "matrix3d", matrix3d);
        setVec3(shader, "cameraPos", &camera.position.x);

        render(window, terrain, terrainSize);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &terrain);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}