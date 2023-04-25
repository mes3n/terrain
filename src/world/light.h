#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>

#include "../math/vector.h"

#define LIGHT_PATH_HEIGHT 200
#define LIGHT_PATH_RADIUS 200

void getLightPos(Vec3 *lightOrigin, double time);

GLuint generateIcoSphere(float radius, int refine, int *sphereSize);

#endif // LIGHT_H
