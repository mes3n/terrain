#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

GLuint loadShaders (const char* vertPath, const char* fragPath);

void setMat4 (GLuint program, const char* name, float* matrix);

#endif  // SHADER_H