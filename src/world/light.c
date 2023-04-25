#include "light.h"

#include "../math/vector.h"
#include "../math/vertex.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdio.h>

void getLightPos(Vec3 *lightOrigin, double time)
{
    lightOrigin->x = LIGHT_PATH_RADIUS * cos(time);
    lightOrigin->y = LIGHT_PATH_HEIGHT;
    lightOrigin->z = LIGHT_PATH_RADIUS * sin(time);
}
// TODO add the smoothness parameter -> refien

GLuint generateIcoSphere(float radius, int refine, int *sphereSize)
{
    size_t verticesCount = (size_t)(4 * 3); // corners of 3 rectangles
    Vertex *vertices = (Vertex *)malloc(verticesCount * sizeof(Vertex));

    size_t indicesCount = (size_t)(20 * 3); // twenty triangles to shape
    GLuint *indices = (GLuint *)malloc(indicesCount * sizeof(GLuint));
    
    float rwidth = ((1.0f + sqrtf(5.0f)) * radius * 0.5);
    float rlength = radius;

    SET_VEC3(vertices[0].position, -rlength, rwidth, 0.0f);
    SET_VEC3(vertices[1].position, rlength, rwidth, 0.0f);
    SET_VEC3(vertices[2].position, -rlength, -rwidth, 0.0f);
    SET_VEC3(vertices[3].position, rlength, -rwidth, 0.0f);

    SET_VEC3(vertices[4].position, 0.0f, -rlength, rwidth);
    SET_VEC3(vertices[5].position, 0.0f, rlength, rwidth);
    SET_VEC3(vertices[6].position, 0.0f, -rlength, -rwidth);
    SET_VEC3(vertices[7].position, 0.0f, rlength, -rwidth);

    SET_VEC3(vertices[8].position, rwidth, 0.0f, -rlength);
    SET_VEC3(vertices[9].position, rwidth, 0.0f, rlength);
    SET_VEC3(vertices[10].position, -rwidth, 0.0f, -rlength);
    SET_VEC3(vertices[11].position, -rwidth, 0.0f, rlength);

    radius = lengthOf(vertices[0].position);

    for (int i = 0; i < verticesCount; i += 1)
    {
        vertices[i].normal = normalize(vertices[i].position);
    }

    // 5 faces around point 0
    SET_ARRAY((&indices[3 * 0]), 0, 11, 5);
    SET_ARRAY((&indices[3 * 1]), 0, 5, 1);
    SET_ARRAY((&indices[3 * 2]), 0, 1, 7);
    SET_ARRAY((&indices[3 * 3]), 0, 7, 10);
    SET_ARRAY((&indices[3 * 4]), 0, 10, 11);

    // 5 adjacent faces
    SET_ARRAY((&indices[3 * 5]), 1, 5, 9);
    SET_ARRAY((&indices[3 * 6]), 5, 11, 4);
    SET_ARRAY((&indices[3 * 7]), 11, 10, 2);
    SET_ARRAY((&indices[3 * 8]), 10, 7, 6);
    SET_ARRAY((&indices[3 * 9]), 7, 1, 8);

    // 5 faces around point 3
    SET_ARRAY((&indices[3 * 10]), 3, 9, 4);
    SET_ARRAY((&indices[3 * 11]), 3, 4, 2);
    SET_ARRAY((&indices[3 * 12]), 3, 2, 6);
    SET_ARRAY((&indices[3 * 13]), 3, 6, 8);
    SET_ARRAY((&indices[3 * 14]), 3, 8, 9);

    // 5 adjacent faces
    SET_ARRAY((&indices[3 * 15]), 4, 9, 5);
    SET_ARRAY((&indices[3 * 16]), 2, 4, 11);
    SET_ARRAY((&indices[3 * 17]), 6, 2, 10);
    SET_ARRAY((&indices[3 * 18]), 8, 6, 7);
    SET_ARRAY((&indices[3 * 19]), 9, 8, 1);

    for (int ref = 0; ref < refine; ref++)
    {

        // Vertex *tempVertices = vertices;
        // vertices = (Vertex *)malloc(verticesCount * 2 * sizeof(Vertex));
        // for (size_t i = 0; i < verticesCount - 1; i++)
        // {
        //     vertices[2 * i] = tempVertices[i];
        //     vertices[2 * i + 1].position = scaleVector(normalize(addVector(tempVertices[i].position, tempVertices[i + 1].position)), 0.5f);
        //     vertices[2 * i + 1].normal = normalize(vertices[2 * i + 1].position);
        // }
        // verticesCount *= 2;

        // GLuint* tempIndices = indices;
        //  indices = (GLuint *)malloc(indicesCount * 4 * sizeof(GLuint));
        // for (size_t i = 0; i < indicesCount; i++)
        // {
        //     indices[i] = 2* tempIndices[i];
        // }
        // indicesCount *= 4;

        // free(tempVertices);
        // free(tempIndices);

        Vertex* oldVertices = vertices;
        vertices = (Vertex*)malloc(verticesCount * 2 * sizeof(Vertex));

        GLuint* oldIndices = indices;
        indices = (GLuint*)malloc(indicesCount * 4 * sizeof(GLuint));

        for (size_t i = 0; i < indicesCount; i += 3) 
        {
            GLuint* face = &oldIndices[i];  // "len" = 3

            printf("vertices for %ld\n", i);
            for (int j = 0; j < 3; j++) 
            {
                vertices[2*face[j]] = oldVertices[face[j]];  // add 1 space between for new vertex
            }
            for (int j = 0; j < 3; j++) 
            {
                vertices[2*face[j] + 1].position = scaleVector(normalize(addVector(
                    vertices[2*face[j]].position, 
                    vertices[2*face[j < 2 ? j + 1 : 0]].position
                    )), radius);
                vertices[2*face[j] + 1].normal = normalize(vertices[2*face[j] + 1].position);
            }

            printf("indices for %ld\n", i);

            SET_ARRAY((&indices[(i+0)*3]), 2*face[0], 2*face[0] + 1, 2*face[2] + 1);
            SET_ARRAY((&indices[(i+1)*3]), 2*face[1], 2*face[1] + 1, 2*face[0] + 1);
            SET_ARRAY((&indices[(i+2)*3]), 2*face[2], 2*face[2] + 1, 2*face[1] + 1);
            SET_ARRAY((&indices[(i+3)*3]), 2*face[0] + 1, 2*face[1] + 1, 2*face[2] + 1);

            // SET_ARRAY((&indices[(i+4)*3]), 2*face[0], 2*face[1], 2*face[2]);

        }

        verticesCount *= 2;
        indicesCount *= 4;

        free(oldVertices);
        free(oldIndices);

    }
    
    printf("sphering done!!\n");

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

    *sphereSize = verticesCount * FLOAT_PER_VERTEX;
    
    return vao;
}
