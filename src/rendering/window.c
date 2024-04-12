#include <glad/glad.h>

#include "window.h"

#include "../math/vector.h"

#include <math.h>
#include <stdio.h>

static void
framebufferSizeCallback(GLFWwindow* window, int width, int heigth)
{
    glViewport(0, 0, width, heigth);
}

static void
cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* camera = glfwGetWindowUserPointer(window);

    float dx = (float)(xpos);
    float dy = (float)(ypos);

    camera->yaw += dx * camera->sensitivity;
    camera->pitch -= dy * camera->sensitivity;

    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;

    camera->forward.x = cosf(camera->yaw + M_PI_2);
    camera->forward.y = 0.0f;
    camera->forward.z = sinf(camera->yaw + M_PI_2);

    glfwSetCursorPos(window, 0, 0);
}

GLFWwindow*
createWindow(const char* name, Camera* camera)
{
    if (!glfwInit())
    {
        const char* err;
        glfwGetError(&err);
        printf("Failed to initialize GLFW: %s\n", err);
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

    GLFWwindow* window
        = glfwCreateWindow(mode->width, mode->height, name, monitor, NULL);
    if (window == NULL)
    {
        const char* err;
        glfwGetError(&err);
        printf("Failed to create GLFW window: %s\n", err);
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSetWindowUserPointer(window, camera);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 0, 0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return NULL;
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CW);

    return window;
}

void
processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    glfwPollEvents();
    Camera* camera = glfwGetWindowUserPointer(window);
    Vec3 velocity = { 0.0f, 0.0f, 0.0f };
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        velocity = addVector(velocity, camera->forward);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        velocity = subVector(velocity, camera->forward);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        velocity = addVector(
            velocity, normalize(cross(camera->forward, camera->worldUp)));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        velocity = subVector(
            velocity, normalize(cross(camera->forward, camera->worldUp)));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        velocity = subVector(velocity, camera->worldUp);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        velocity = addVector(velocity, camera->worldUp);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->speed = 1.2f;
    else
        camera->speed = 0.4f;

    moveCamera(camera, velocity);
}

void
render(GLFWwindow* window, const Renderable* toRender)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    while (toRender != NULL)
    {
        glUseProgram(toRender->shader);
        glBindVertexArray(toRender->vao);
        glDrawElements(GL_TRIANGLES, toRender->size, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
        toRender = toRender->next;
    }

    glfwSwapBuffers(window);
}
