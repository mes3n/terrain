#include <glad/glad.h>  // glad must always be included before glfw

#include "window.h"
#include "matrix.h"

#include <stdio.h>

static void framebuffer_size_callback (GLFWwindow* window, int width, int heigth) 
{
    glViewport(0, 0, width, heigth);
}

GLFWwindow* createWindow (const char* name) 
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "OpenGL", monitor, NULL);
    if (window == NULL) 
    {
        printf("Failed to create GLFW window\n");
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        printf("Failed to initialize GLAD\n");
        return NULL;
    }

    glEnable(GL_DEPTH_TEST);
    return window;
}

void processInput (GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void getPerspective (GLFWwindow* window, float* matrix) 
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    mat_perspective(matrix, 45.0f, (float)width/(float)height, 0.1f, 100.0f);
}
