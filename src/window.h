#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

GLFWwindow* createWindow (const char* name);

void processInput (GLFWwindow* window);
void getPerspective (GLFWwindow* window, float* matrix);

#endif  // WINDOW_H
