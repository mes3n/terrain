#ifndef VECTOR_H
#define VECTOR_H

#define SET_VEC3(vec, a, b, c) \
    vec.x = a;                 \
    vec.y = b;                 \
    vec.z = c;

typedef struct
{
    float x;
    float y;
    float z;
} Vec3;

float
lengthOf(const Vec3 vector);
Vec3
normalize(const Vec3 vector);

Vec3
cross(const Vec3 v1, const Vec3 v2);

Vec3
addVector(const Vec3 v1, const Vec3 v2);
Vec3
subVector(const Vec3 v1, const Vec3 v2);

Vec3
scaleVector(const Vec3 v, float scale);

#endif // VECTOR_H
