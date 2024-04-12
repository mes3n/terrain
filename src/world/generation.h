#ifndef GENERATION_H
#define GENERATION_H

#include <glad/glad.h>

typedef struct
{
    const int x, z;
    GLuint vao;
} Chunk;

static const int CHUNK_SIZE = 64;

typedef struct
{
    Chunk chunks[21];
    int activeChunks;
} Terrain;

void
initTerrain(Terrain* terrain);

GLuint
generateChunk(const int x, const int y, int* terrainSize);

#endif // GENERATION_H
