#include "matrix.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926536
#endif  // M_PI


void mat_translate(float *matrix, Vec3 v) {
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = v.x;
    matrix[13] = v.y;
    matrix[14] = v.z;
    matrix[15] = 1.0f;
}

void mat_rotate(float *matrix, Vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sinf(angle);
    float c = cosf(angle);
    float m = 1 - c;

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    matrix[0] = m * x * x + c;
    matrix[1] = m * x * y - z * s;
    matrix[2] = m * z * x + y * s;
    matrix[3] = 0.0f;
    matrix[4] = m * x * y + z * s;
    matrix[5] = m * y * y + c;
    matrix[6] = m * y * z - x * s;
    matrix[7] = 0.0f;
    matrix[8] = m * z * x - y * s;
    matrix[9] = m * y * z + x * s;
    matrix[10] = m * z * z + c;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

void mat_multiply(float *matrix, const float *m1, const float *m2) 
{
    float result[16];
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            int index = c * 4 + r;
            float total = 0;
            for (int i = 0; i < 4; i++) {
                int p = i * 4 + r;
                int q = c * 4 + i;
                total += m1[p] * m2[q];
            }
            result[index] = total;
        }
    }
    for (unsigned int i = 0; i < 16; i++) 
    {
        matrix[i] = result[i];
    }
}

void mat_identity (float* matrix) 
{
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

void mat_frustum (float *matrix, float left, float right, float bottom, float top, float znear, float zfar) {
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0f;
}

void mat_perspective (float *matrix, float fov, float aspect, float znear, float zfar) {
    float ymax, xmax;
    ymax = znear * tanf(fov * M_PI / 360.0f);
    xmax = ymax * aspect;
    mat_frustum(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void set_matrix_3d (float *matrix, GLFWwindow* window, const Camera camera, float fov, int radius)
{   
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    Vec3 rotationAxis;
    rotationAxis.x = cosf(camera.yaw);
    rotationAxis.y = 0.0f;
    rotationAxis.z = sinf(camera.yaw);

    float m1[16];
    float m2[16];
    float aspect = (float)width / (float)height;
    float znear = 0.1;
    float zfar = radius * 32 + 64;
    mat_identity(m1);
    mat_translate(m2, camera.position);
    mat_multiply(m1, m2, m1);
    mat_rotate(m2, rotationAxis, camera.pitch);
    mat_multiply(m1, m2, m1);
    mat_rotate(m2, camera.worldUp, -camera.yaw);
    mat_multiply(m1, m2, m1);
    mat_perspective(m2, fov, aspect, znear, zfar);
    mat_multiply(m1, m2, m1);
    mat_identity(matrix);
    mat_multiply(matrix, m1, matrix);
}
