#ifndef MATRIX_H
#define MATRIX_H

#define PI 3.14159265359

typedef struct {
    float x;
    float y;
    float z;
} vec3;

typedef float mat4[16];

// void mat_translate(float* matrix, vec3 vector);
// void mat_rotate

void mat_identity(float* matrix);
void mat_frustum (float *matrix, float left, float right, float bottom, float top, float znear, float zfar);
void mat_perspective (float *matrix, float fov, float aspect, float znear, float zfar);


#endif  // MATRIX_H
