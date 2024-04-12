#include "light.h"

#include "../math/vector.h"
#include "../math/vertex.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>

void
getLightPos(Vec3* lightOrigin, double time)
{
    lightOrigin->x = LIGHT_PATH_RADIUS * cos(time);
    lightOrigin->y = LIGHT_PATH_HEIGHT;
    lightOrigin->z = LIGHT_PATH_RADIUS * sin(time);
}

GLuint
generateUVSphere(float radius, int* sphereSize)
{
    const size_t hSteps = 100, vSteps = 100;

    // top and bottom take 1 index each
    size_t verticesCount = (vSteps - 1) * hSteps + 2;
    Vertex* vertices = (Vertex*)malloc(verticesCount * sizeof(Vertex));

    // only one row of triangle at top and bottom
    size_t indicesCount = (vSteps - 1) * hSteps * 2 * 3;
    GLuint* indices = (GLuint*)malloc(indicesCount * sizeof(GLuint));

    size_t counter;
    counter = 0;
    for (size_t y = 1; y < vSteps; y++)
    {
        float ay = M_PI * ((float)y / (float)vSteps);
        for (size_t x = 0; x < hSteps; x++)
        {
            float ax = 2 * M_PI * ((float)x / (float)hSteps);
            Vertex newVertex;
            newVertex.position.x = radius * sinf(ay) * sinf(ax);
            newVertex.position.y = radius * cosf(ay);
            newVertex.position.z = radius * sinf(ay) * cosf(ax);
            newVertex.normal = normalize(newVertex.position);
            vertices[counter++] = newVertex;
        }
    }
    const Vertex top = (Vertex) { (Vec3) {
                                      .x = 0.0f,
                                      .y = radius,
                                      .z = 0.0f,
                                  },
        (Vec3) {
            .x = 0.0f,
            .y = 1.0f,
            .z = 0.0f,
        } };
    const Vertex bottom = (Vertex) { (Vec3) {
                                         .x = 0.0f,
                                         .y = -radius,
                                         .z = 0.0f,
                                     },
        (Vec3) {
            .x = 0.0f,
            .y = -1.0f,
            .z = 0.0f,
        } };
    const size_t topIndex = counter;
    vertices[counter++] = top;
    const size_t bottomIndex = counter;
    vertices[counter++] = bottom;

    counter = 0;
    size_t i = 0;
    for (i = 0; i < hSteps; i++) // generate indices from heightmap
    {
        SET_ARRAY((&indices[counter]), i, topIndex, (i + 1) % hSteps);
        counter += 3;
    }
    for (i = 0; i < verticesCount - hSteps - 2;
         i++) // generate indices from heightmap
    {
        const size_t rs = (i + 1) % hSteps != 0 ? i + 1 : i + 1 - hSteps;
        SET_ARRAY((&indices[counter + 0]), i, rs, rs + hSteps);
        SET_ARRAY((&indices[counter + 3]), i, rs + hSteps, i + hSteps);
        counter += 6;
    }
    for (i = verticesCount - hSteps - 2; i < verticesCount - 2;
         i++) // generate indices from heightmap
    {
        SET_ARRAY((&indices[counter]), i,
            (i + 1) % hSteps != 0 ? i + 1 : i + 1 - hSteps, bottomIndex);
        counter += 3;
    }

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices,
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(GLuint),
        indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)(offsetof(Vertex, normal)));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(vertices);
    free(indices);

    *sphereSize = verticesCount * FLOAT_PER_VERTEX;

    return vao;
}
