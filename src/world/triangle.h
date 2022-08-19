#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glad/glad.h>

static const GLfloat triangleVertices[] = {
    0.0f, 0.0f,  0.5f,  1.0f, 0.0f, 0.0f, // top
   -0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f, // bottom right
    0.5f, 0.0f, -0.5f,  0.0f, 0.0f, 1.0f, // bottom left
};

#endif  // TRIANGLE_H
