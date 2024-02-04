#include "vector.h"

#include <math.h>

void
setVector3(float* arr, float x, float y, float z)
{
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
}

float
lengthOf(const Vec3 vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y
        + vector.z * vector.z);
}

Vec3
normalize(const Vec3 vector)
{
    Vec3 result;

    float length = lengthOf(vector);

    if (length != 0)
    {
        result.x = vector.x / length;
        result.y = vector.y / length;
        result.z = vector.z / length;
    }
    else
    {
        result.x = 0;
        result.y = 0;
        result.z = 0;
    }

    return result;
}

Vec3
cross(const Vec3 v1, const Vec3 v2)
{
    Vec3 result;

    result.x = v1.y * v2.z - v2.y * v1.z;
    result.y = v1.z * v2.x - v2.z * v1.x;
    result.z = v1.x * v2.y - v2.x * v1.y;

    return result;
}

Vec3
addVector(const Vec3 v1, const Vec3 v2)
{
    Vec3 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

Vec3
subVector(const Vec3 v1, const Vec3 v2)
{
    Vec3 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

Vec3
scaleVector(const Vec3 v, float scale)
{
    Vec3 result;

    result.x = v.x * scale;
    result.y = v.y * scale;
    result.z = v.z * scale;

    return result;
}