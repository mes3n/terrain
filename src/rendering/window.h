#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include "camera.h"

typedef struct {
    GLuint vao;
    int size;
    GLuint shader;
    
} Renderable;


typedef struct Camera Camera;

GLFWwindow* createWindow (const char* name, Camera* camera);

void render (GLFWwindow* window, Renderable* toRender, int renderCount);
void processInput (GLFWwindow* window);

#endif  // WINDOW_H
