#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include "camera.h"

typedef struct {

    GLuint vao;
    int size;
    
} renderable;


typedef struct Camera Camera;

GLFWwindow* createWindow (const char* name, Camera* camera);

void render (GLFWwindow* window, renderable* toRender, int renderCount);
void processInput (GLFWwindow* window);

#endif  // WINDOW_H
