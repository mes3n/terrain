#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "math/math.h"
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
    GLuint terrain = generateTerrain(0.0f, -50.0f, 0.0f, 500, 500, 128.0f, &terrainSize);
    
    glUseProgram(shader);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glLineWidth(5.0f);

    float matrix3d[16];

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glUseProgram(shader);

        set_matrix_3d(matrix3d, window, camera, 45.0f, 100);
        setMat4(shader, "matrix", matrix3d);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw(terrain, terrainSize);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &terrain);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}