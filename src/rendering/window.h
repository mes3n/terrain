#ifndef WINDOW_H
#define WINDOW_H

#include "camera.h"
#include "renderable.h"

#include <GLFW/glfw3.h>

GLFWwindow*
createWindow(const char* name, Camera* camera);

void
render(GLFWwindow* window, const Renderable* toRender);

void
processInput(GLFWwindow* window);

#endif // WINDOW_H
