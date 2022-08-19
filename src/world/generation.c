#include "generation.h"

#include "noise.h"
#include "triangle.h"

#include <stdlib.h>
#include <stdio.h>

static void setVertex(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b) 
{
    vertices[0] = x;
    vertices[1] = y;
    vertices[2] = z;
    vertices[3] = r;  // R
    vertices[4] = g;  // G
    vertices[5] = b;  // B
}

static void setIndices(GLuint* indeices, GLuint i, GLuint j, GLuint k) 
{
    indeices[0] = i;
    indeices[1] = j;
    indeices[2] = k;
}

static int equal(float n1, float n2, float p) 
{
    if (n1 - n2 > p)
        return 0;
    else if (n1 - n2 < -p)
        return 0;
    else
        return 1;
} 

GLuint generateTerrain (float centerx, float centery, float centerz, int width, int depth, float amplitude, int* terrainSize) 
{
    GLfloat* vertices;
    GLuint* indices; 
    
    static const float detail = 1.0f;  // must be 1 atm

    const unsigned int fpv = 6; 
    const size_t verticesCount = (size_t)(width * depth *  fpv);
    const size_t indicesCount = (size_t)((width - 1) * (depth - 1) * 2 * 3);
    vertices = (GLfloat*)calloc(verticesCount, sizeof(GLfloat));
    indices = (GLuint*)calloc(indicesCount, sizeof(GLuint));
    
    const float startx = centerx - width * 0.5f; 
    const float startz = centerz - depth * 0.5f;
    const float miny = centery - amplitude * 0.5;
    // const float maxy = centery + amplitude * 0.5;

    size_t i = 0;
    for (float x = startx; x < startx + width; x += detail) {
        for (float z = startz; z < startz + depth; z += detail) 
        {               
            float y = noise(x, z, miny, amplitude, 2.0f); 

            float r = (y - miny) / amplitude;
            float g = (y - miny) / amplitude;
            float b = (y - miny) / amplitude;

            setVertex(&vertices[i], x, y, z, r, g, b);
            i += fpv;
        }
    }
    i = 0;
    for (size_t ii = 0; ii < verticesCount / fpv; ii++) 
    {   
        if (!equal(vertices[ii*fpv], startx, 0.01) && !equal(vertices[(ii*fpv)+2], startz, 0.01))
        {
            setIndices(&indices[i + 0], ii, ii - width, ii - width - 1);
            setIndices(&indices[i + 3], ii, ii - width - 1, ii - 1);
            i += 6;
        }
    }

    // printf("indices\n");
    // for (int a = 0; a < indicesCount; a += 3) {
    //     printf("%d: %d, %d, %d\n", a / 3, indices[a], indices[a+1], indices[a+2]);
    // }
    // printf("vertices\n");
    // for (int a = 0; a < verticesCount; a += fpv) {
    //     printf("%d: %f, %f, %f, %f, %f, %f\n", a / fpv, vertices[a+0], vertices[a+1], vertices[a+2], vertices[a+3], vertices[a+4], vertices[a+5]);
    // }

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(vertices);
    free(indices);

    *terrainSize = indicesCount;
    return vao;
}

void draw (GLuint vao, int size) 
{
    // printf("size: %ld\n", size);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}