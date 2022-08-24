#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "math/matrix.h"
#include "rendering/rendering.h"

#include "world/generation.h"
#include "world/noise.h"


void screenshot (GLFWwindow* window) 
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const int width = mode->width;
    const int height = mode->height;
    size_t size = width * height * 3;

    unsigned char data[size];

    glReadBuffer(GL_BACK);
    glReadnPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, size, data);

    FILE* file = fopen("image.tga", "w");
    short header[] = {0, 2, 0, 0, 0, 0, (short) width, (short) height, 24};

    fwrite(&header, sizeof(header), 1, file);
    fwrite(data, size, 1, file);

    fclose(file);
}


int main (void) 
{   

    Camera camera;
    initCamera(&camera);

    GLFWwindow* window = createWindow("OpenGL", &camera);
    if (window == NULL)
        return -1;

    GLuint shader = loadShaders("shaders/basic.vert", "shaders/basic.frag");


    int terrainSize;
    GLuint terrain = generateTerrain(0.0f, -10.0f, 0.0f, 1000, 1000, 64.0f, &terrainSize);
    
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