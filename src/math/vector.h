#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
    float z;
} Vec3;


float lenghtOf (const Vec3 vector);
Vec3 normalize (const Vec3 vector);

Vec3 cross (const Vec3 v1, const Vec3 v2);

Vec3 addVector (const Vec3 v1, const Vec3 v2);
Vec3 subVector (const Vec3 v1, const Vec3 v2);

Vec3 scaleVector (const Vec3 v, float scale);


#endif  // VECTOR_H
