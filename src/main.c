#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "math/matrix.h"

#include "rendering/window.h"
#include "rendering/camera.h"
#include "rendering/shader.h"

#include "world/generation.h"
#include "world/light.h"
#include "world/noise.h"

void screenshot(GLFWwindow *window)
{
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const int width = mode->width;
    const int height = mode->height;
    size_t size = width * height * 3;

    unsigned char data[size];

    glReadBuffer(GL_BACK);
    glReadnPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, size, data);

    FILE *file = fopen("image.tga", "w");
    short header[] = {0, 2, 0, 0, 0, 0, (short)width, (short)height, 24};

    fwrite(&header, sizeof(header), 1, file);
    fwrite(data, size, 1, file);

    fclose(file);
}

int main(void)
{
    Camera camera;
    initCamera(&camera);

    GLFWwindow *window = createWindow("OpenGL", &camera);
    if (window == NULL)
        return 1;

    renderable toRender[10];
    int renderCount = 0;

    GLuint shader = loadShaders("shaders/basic.vert", "shaders/basic.frag");

    int terrainSize;
    GLuint terrain = generateTerrain(500, 500, &terrainSize);
    toRender[0] = (renderable){.vao = terrain, .size = terrainSize};
    renderCount += 1;

    Vec3 lightOrigin;
    getLightPos(&lightOrigin, glfwGetTime());

    // int sphereSize;
    // GLuint sphere = generateIcoSphere(10.0f, 1, &sphereSize);
    // toRender[1] = (renderable){.vao = sphere, .size = sphereSize};
    // renderCount += 1;

    // int sphereSize2;
    // GLuint sphere2 = generateIcoSphere(15.0f, 0, &sphereSize2);
    // toRender[2] = (renderable){.vao = sphere2, .size = sphereSize2};
    // renderCount += 1;

    glUseProgram(shader);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glLineWidth(5.0f);

    glEnable(GL_PROGRAM_POINT_SIZE);

    float matrix3d[16];

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        getView(window, camera, matrix3d);
        getLightPos(&lightOrigin, glfwGetTime());

        setMat4(shader, "matrix3d", matrix3d);
        setVec3(shader, "cameraPos", &camera.position.x);
        setVec3(shader, "lightOrigin", &lightOrigin.x);

        render(window, toRender, renderCount);

        glfwPollEvents();
    }
    
    // glDeleteVertexArrays(1, &sphere);
    glDeleteVertexArrays(1, &terrain);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}