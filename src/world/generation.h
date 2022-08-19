#ifndef GENERATION_H
#define GENERATION_H

#include <glad/glad.h>


GLuint generateTerrain (float centerx, float centery, float centerz, int width, int depth, float amplitude, int* terrainSize);

void draw (GLuint vao, int size);

#endif  // GENERATION_H
