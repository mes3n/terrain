#include "generation.h"

#include "../deps/noise/noise.h"
#include "../math/vector.h"

#include <stdlib.h>
#include <stdio.h>


static void setVector3(GLfloat* arr, GLfloat x, GLfloat y, GLfloat z) 
{
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
}

static void setIndices(GLuint* indices, GLuint x, GLuint y, GLuint z) 
{
    indices[0] = x;
    indices[1] = y;
    indices[2] = z;
}

static int nearby(float n1, float n2, float p) 
{
    if (n1 - n2 > p)
        return 0;
    else if (n1 - n2 < -p)
        return 0;
    else
        return 1;
}

static void nearbyTriangles (GLuint* triangles, int* trianglesCount, const GLuint* indices, int indicesCount, GLuint index)  
// find all triangles using vertex at index
{
    int counter = 0;
    for (int i = 0; i < indicesCount; i += 3) {
        if (indices[i+0] == index || indices[i+1] == index || indices[i+2] == index)
        {
            triangles[counter] = i;
            counter++;
        }
        if (counter >= 6) {
            *trianglesCount = counter;
            return;
        }
    }
    *trianglesCount = counter;
}

GLuint generateTerrain (float centerx, float centery, float centerz, const int width, const int depth, float amplitude, int* terrainSize)  // width = depth atm
{
    GLuint* indices;
    GLfloat* vertices;
    
    const size_t indicesCount = (size_t)((width - 1) * (depth - 1) * 2 * 3);
    indices = (GLuint*)calloc(indicesCount, sizeof(GLuint));

    const unsigned int fpv = 6; 
    const size_t verticesCount = (size_t)(width * depth * fpv);
    vertices = (GLfloat*)calloc(verticesCount, sizeof(GLfloat));

    const float startx = centerx - width * 0.5f; 
    const float startz = centerz - depth * 0.5f;

    size_t counter;
    counter = 0;
    for (float x = startx; x < startx + width; x++) {  // generate noise based heightmap
        for (float z = startz; z < startz + depth; z++) 
        {   
            float y = simplex2(x, z, 7, 2.0f, 0.4f) * amplitude + centery; 
            setVector3(&vertices[counter], x, y, z);
            counter += fpv;
        }
    }
    counter = 0;
    for (size_t i = 0; i < verticesCount; i += fpv)  // generate indices from heightmap
    {   
        if (!nearby(vertices[i+0], startx, 0.01) && !nearby(vertices[i+2], startz, 0.01))  // if not at start edge
        {   
            size_t vi = i / fpv;
            setIndices(&indices[counter + 0], vi, vi - depth, vi - depth - 1);
            setIndices(&indices[counter + 3], vi, vi - depth - 1, vi - 1);
            counter += 6;
        }
    }
    counter = 0;
    for (size_t i = 0; i < verticesCount; i += fpv) 
    {   
        // int column = (i / 6) / depth;
        // int row = (i / 6) % depth;
        // int trianglesCount = 0;  // found amount of triangles
        // size_t vi = i / 6;
        // if (column == 0 || column == width - 1 || row == 0 || row == depth - 1) 
        // {
        //     if ((column == 0 && row == 0) || (row == 0 && column == width - 1)) 
        //     {
        //         trianglesCount = 1;
        //         GLuint triangles[3] = {
        //             vi, 
        //         };
        //     }
        //     else if ((column == 0 && row == 0) || (row == depth - 1 && column == width - 1)) 
        //     {
        //         trianglesCount = 2;
        //     }
        //     else 
        //     {
        //         trianglesCount = 3;
        //     }
        // }
        // else 
        // {
        //     trianglesCount = 6;
        // }
        int trianglesCount = 0;  // found amount of triangles
        GLuint triangles[6];  // indices of found triangles
        nearbyTriangles(triangles, &trianglesCount, indices, indicesCount, i / fpv);


        Vec3 corners[3], normal;
        setVector3(&normal.x, 0.0f, 0.0f, 0.0f);
        for (int ii = 0; ii < trianglesCount; ii++) {
            for (size_t j = 0; j < 3; j++) 
            {
                GLuint vi = indices[triangles[ii]+j] * fpv;  // index of vertex
                corners[j].x = vertices[vi+0];
                corners[j].y = vertices[vi+1];
                corners[j].z = vertices[vi+2];
            }
            normal = addVector(normal, normalize(cross(subVector(corners[0], corners[2]), subVector(corners[0], corners[1]))));
        }
        if (trianglesCount != 0)
            normal = scaleVector(normal, 1.0f/trianglesCount);
        setVector3(&vertices[i+3], normal.x, normal.y, normal.z);  // set normal component of each vertice
    }

    // printf("indices:\n");
    // for (int a = 0; a < indicesCount; a += 3) 
    // {
    //     printf("%d: %d, %d, %d\n", a, indices[a+0], indices[a+1], indices[a+2]);
    // }    
    // printf("vertices:\n");
    // for (int a = 0; a < verticesCount; a += fpv) 
    // {
    //     printf("%d: %f, %f, %f\n", a, vertices[a+0], vertices[a+1], vertices[a+2]);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, fpv * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, fpv * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(vertices);
    free(indices);

    *terrainSize = verticesCount;
    return vao;
}
