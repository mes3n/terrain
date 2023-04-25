#include "generation.h"

// #include "../deps/noise/noise.h"
#include "noise.h"

#include "../math/vector.h"
#include "../math/vertex.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define FLOAT_EQ(x, y, h) \
    -h < x - y && x - y > h

GLuint generateTerrain(const int width, const int depth, int *terrainSize) // width = depth atm
{
    const size_t verticesCount = (size_t)(width * depth);
    Vertex *vertices = (Vertex *)malloc(verticesCount * sizeof(Vertex));

    const size_t indicesCount = (size_t)((width - 1) * (depth - 1) * 2 * 3);
    GLuint *indices = (GLuint *)malloc(indicesCount * sizeof(GLuint));

    const float startx = -width * 0.5f;
    const float startz = -depth * 0.5f;

    size_t counter;
    counter = 0;
    for (float x = startx; x < startx + width; x += 1.0f)
    { // generate noise based heightmap
        for (float z = startz; z < startz + depth; z += 1.0f)
        {
            // float y = simplex2(x, z, 7, 2.0f, 0.4f) * amplitude + centery;
            float y;
            if (x < 0.0f) {
                y = noise(x, z, 1.0f, 10000.0f);
            }
            else {
              y = noise(x, z, 1.0f, 0.01f);

            }
            SET_VEC3(vertices[counter].position, x, y, z);
            // SET_VEC3(vertices[counter].position, x, 0, z);
            // SET_VEC3(vertices[counter].normal, y, y, y);
            
            counter += 1;
        }
    }
    counter = 0;
    for (size_t i = 0; i < verticesCount; i += 1) // generate indices from heightmap
    {
        if (FLOAT_EQ(vertices[i].position.x, startx, 0.01) && FLOAT_EQ(vertices[i].position.z, startz, 0.01)) // if not at start edge
        {
            SET_ARRAY((&indices[counter + 0]), i, i - depth, i - depth - 1);
            SET_ARRAY((&indices[counter + 3]), i, i - depth - 1, i - 1);
            counter += 6;
        }
    }

    counter = 0;
    for (size_t i = 0; i < verticesCount; i += 1)
    {
        int column = i % depth;
        int row = i / depth;
        int trianglesCount = 0; // found amount of triangles
        GLuint *triangles;
        if (column == 0 || column == width - 1 || row == 0 || row == depth - 1)
        {
            if (column == 0)
            {
                if (row == depth - 1) // corner top left
                {
                    trianglesCount = 1;
                    const GLuint tempTriangles[3] = {
                        i,
                        i + 1,
                        i - width,
                    };
                    triangles = (GLuint *)malloc(sizeof(tempTriangles));
                    memcpy(triangles, tempTriangles, sizeof(tempTriangles));
                }
                else if (row == 0) // corner bottom left
                {
                    trianglesCount = 2;
                    const GLuint tempTriangles[6] = {
                        i,
                        i + 1,
                        i + 1 + width,
                        i,
                        i + width,
                        i + 1 + width,
                    };
                    triangles = (GLuint *)malloc(sizeof(tempTriangles));
                    memcpy(triangles, tempTriangles, sizeof(tempTriangles));
                }
                else
                {
                    trianglesCount = 3;
                    const GLuint tempTriangles[9] = {
                        i,
                        i + width,
                        i + width + 1,
                        i,
                        i + width + 1,
                        i + 1,
                        i,
                        i + 1,
                        i - width,
                    };
                    triangles = (GLuint *)malloc(sizeof(tempTriangles));
                    memcpy(triangles, tempTriangles, sizeof(tempTriangles));
                }
            }
            else if (row == 0)
            {
                if (column == width - 1) // corner bottom right
                {
                    trianglesCount = 1;
                    const GLuint tempTriangles[3] = {
                        i,
                        i - 1,
                        i + width,
                    };
                    triangles = (GLuint *)malloc(sizeof(tempTriangles));
                    memcpy(triangles, tempTriangles, sizeof(tempTriangles));
                }
                else
                {
                    trianglesCount = 3;
                    const GLuint tempTriangles[9] = {
                        i,
                        i - 1,
                        i + width,
                        i,
                        i + width,
                        i + width + 1,
                        i,
                        i + width + 1,
                        i + 1,
                    };
                    triangles = (GLuint *)malloc(sizeof(tempTriangles));
                    memcpy(triangles, tempTriangles, sizeof(tempTriangles));
                }
            }
            else if (column == width - 1)
            {
                if (row == depth - 1) // corner top rigth
                {
                    trianglesCount = 2;
                    const GLuint tempTriangles[6] = {
                        i,
                        i - 1,
                        i - 1 - width,
                        i,
                        i - width,
                        i - 1 - width,
                    };
                    triangles = (GLuint *)malloc(sizeof(tempTriangles));
                    memcpy(triangles, tempTriangles, sizeof(tempTriangles));
                }
                else
                {
                    trianglesCount = 3;
                    const GLuint tempTriangles[9] = {
                        i,
                        i - width,
                        i - width - 1,
                        i,
                        i - width - 1,
                        i - 1,
                        i,
                        i - 1,
                        i + width,
                    };
                    triangles = (GLuint *)malloc(sizeof(tempTriangles));
                    memcpy(triangles, tempTriangles, sizeof(tempTriangles));
                }
            }
            else if (row == depth - 1)
            {
                trianglesCount = 3;
                const GLuint tempTriangles[9] = {
                    i,
                    i + 1,
                    i - width,
                    i,
                    i - width,
                    i - width - 1,
                    i,
                    i - width - 1,
                    i - 1,
                };
                triangles = (GLuint *)malloc(sizeof(tempTriangles));
                memcpy(triangles, tempTriangles, sizeof(tempTriangles));
            }
        }
        else
        {
            trianglesCount = 6;
            const GLuint tempTriangles[18] = {
                i,
                i + width,
                i + width + 1,
                i,
                i + width + 1,
                i + 1,
                i,
                i + 1,
                i - width,
                i,
                i - width,
                i - width - 1,
                i,
                i - width - 1,
                i - 1,
                i,
                i - 1,
                i + width,
            };
            triangles = (GLuint *)malloc(sizeof(tempTriangles));
            memcpy(triangles, tempTriangles, sizeof(tempTriangles));
        }

        Vec3 corners[3], normal;
        SET_VEC3(normal, 0.0f, 0.0f, 0.0f);
        for (int ii = 0; ii < trianglesCount * 3; ii += 3)
        {
            for (size_t j = 0; j < 3; j++)
            {
                corners[j] = vertices[triangles[ii + j]].position;
            }
            normal = addVector(normal, normalize(cross(subVector(corners[0], corners[2]), subVector(corners[0], corners[1]))));
        }
        if (trianglesCount != 0)
            normal = scaleVector(normal, 1.0f / trianglesCount);
        vertices[i].normal = normal; // set normal component of each vertice

        free(triangles);
    }

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal)));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(vertices);
    free(indices);

    *terrainSize = verticesCount * FLOAT_PER_VERTEX;
    return vao;
}
