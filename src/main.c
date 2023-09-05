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

    Renderable toRender[10];
    int renderCount = 0;

    GLuint basicShader = loadShaders("shaders/basic.vert", "shaders/basic.frag");
    GLuint lightShader = loadShaders("shaders/light.vert", "shaders/light.frag");

    int terrainSize;
    GLuint terrain = generateTerrain(500, 500, &terrainSize);
    toRender[renderCount++] = (Renderable){.vao = terrain, .size = terrainSize, .shader = basicShader};

    Vec3 lightOrigin;
    getLightPos(&lightOrigin, glfwGetTime());

    // int icoSphereSize;
    // GLuint icoSphere = generateIcoSphere(10.0f, 0, &icoSphereSize);
    // toRender[renderCount++] = (Renderable){.vao = icoSphere, .size = icoSphereSize};

    int uvSphereSize;
    GLuint uvSphere = generateUVSphere(5.0f, &uvSphereSize);
    toRender[renderCount++] = (Renderable){.vao = uvSphere, .size = uvSphereSize, .shader = lightShader};

    glUseProgram(basicShader);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glLineWidth(5.0f);

    glEnable(GL_PROGRAM_POINT_SIZE);

    float matrix3d[16];

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        getView(window, camera, matrix3d);
        getLightPos(&lightOrigin, glfwGetTime());

        setMat4(basicShader, "matrix3d", matrix3d);
        setVec3(basicShader, "cameraPos", &camera.position.x);
        setVec3(basicShader, "lightOrigin", &lightOrigin.x);
        setMat4(lightShader, "matrix3d", matrix3d);
        setVec3(lightShader, "lightOrigin", &lightOrigin.x);

        render(window, toRender, renderCount);

        glfwPollEvents();
    }

    // for (int i = 0; i < renderCount; i++) {
    //     glDeleteVertexArrays(1, toRender[i].vao);
    // }
    glDeleteProgram(basicShader);
    glDeleteProgram(lightShader);
    glfwTerminate();
    return 0;
}