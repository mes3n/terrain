#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "math/vector.h"
#include "rendering/camera.h"
#include "rendering/renderable.h"
#include "rendering/shader.h"
#include "rendering/window.h"

#include "world/generation.h"
#include "world/light.h"
#include "world/noise.h"

#include <stdio.h>

// void screenshot() {
//     const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//     const int width = mode->width;
//     const int height = mode->height;
//     size_t size = width * height * 3;
//
//     unsigned char data[size];
//
//     glReadBuffer(GL_BACK);
//     glReadnPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, size,
//     data);
//
//     FILE *file = fopen("image.tga", "w");
//     short header[] = {0, 2, 0, 0, 0, 0, (short)width, (short)height, 24};
//
//     fwrite(&header, sizeof(header), 1, file);
//     fwrite(data, size, 1, file);
//
//     fclose(file);
// }

int
main(void)
{
    Camera camera;
    initCamera(&camera);

    GLFWwindow* window = createWindow("OpenGL", &camera);
    if (window == NULL)
        return 1;

    GLuint basicShader = loadShaders("shaders/basic.vert", "shaders/basic.frag");
    GLuint lightShader = loadShaders("shaders/light.vert", "shaders/light.frag");

    Renderable* world = NULL;

    perlinInit();

    int terrainSize;
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            GLuint chunk = generateChunk(i, j, &terrainSize);
            addRenderable(&world, basicShader, chunk, terrainSize);
        }
    }

    int uvSphereSize;
    GLuint uvSphere = generateUVSphere(5.0f, &uvSphereSize);
    addRenderable(&world, lightShader, uvSphere, uvSphereSize);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glLineWidth(5.0f);
    //
    // glEnable(GL_PROGRAM_POINT_SIZE);

    float matrix3d[16];
    Vec3 lightOrigin;
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

        render(window, world);
    }

    Renderable* head = world;
    while (head != NULL)
    {
        glDeleteVertexArrays(1, &head->vao);
        head = head->next;
    }
    clearRenderable(world);

    glDeleteProgram(basicShader);
    glDeleteProgram(lightShader);

    glfwTerminate();
    return 0;
}
