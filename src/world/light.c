#include "light.h"

#include <math.h>

void getLightPos(Vec3* lightOrigin, double time) {
    lightOrigin->x = LIGHT_PATH_RADIUS*cos(time);
    lightOrigin->y = LIGHT_PATH_HEIGHT;
    lightOrigin->z = LIGHT_PATH_RADIUS*sin(time);
}