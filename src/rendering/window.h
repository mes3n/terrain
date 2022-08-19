#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include "camera.h"

typedef struct Camera Camera;

GLFWwindow* createWindow (const char* name, Camera* camera);
void processInput (GLFWwindow* window);

#endif  // WINDOW_H
