#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

#include "../rendering/camera.h"
#include "../rendering/window.h"

void
mat_translate(float* matrix, Vec3 vector);
void
mat_rotate(float* matrix, Vec3 axis, float angle);

void
mat_multiply(float* matrix, const float* m1, const float* m2);

void
mat_identity(float* matrix);
void
mat_frustum(float* matrix, float left, float right, float bottom,
    float top, float znear, float zfar);
void
mat_perspective(float* matrix, float fov, float aspect, float znear,
    float zfar);

void
set_matrix_3d(float* matrix, GLFWwindow* window, const Camera camera,
    float fov, int radius);

#endif // MATRIX_H
