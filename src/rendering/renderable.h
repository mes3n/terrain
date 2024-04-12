#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <GLFW/glfw3.h>

typedef struct Renderable
{
    GLuint shader;
    GLuint vao;
    int size;

    struct Renderable* next;
} Renderable;

int
addRenderable(Renderable** head, const GLuint shader, const GLuint vao,
    const int size);

void
removeRenderable(Renderable** head, const GLuint vao);

void
clearRenderable(Renderable* head);

#endif // RENDERABLE_H
