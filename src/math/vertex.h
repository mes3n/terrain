#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

#define FLOAT_PER_VERTEX 6

#define SET_ARRAY(arr, a, b, c) \
    arr[0] = a;                 \
    arr[1] = b;                 \
    arr[2] = c;

typedef struct
{
    Vec3 position;
    Vec3 normal;
    
} Vertex;

#endif // VERTEX_H
