#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vector.h"

#include "window.h"

typedef struct Camera {
    Vec3 position;
    Vec3 facing;
    Vec3 worldUp;

    Vec3 forward;

    float speed;
    float sensitivity;

    float pitch;
    float yaw;

} Camera;

void initCamera(Camera* camera);
void moveCamera (Camera* camera, Vec3 movement);

void getView (GLFWwindow* window, const Camera camera, float* matrix);

#endif  // CAMERA_H
