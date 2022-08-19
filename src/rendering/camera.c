#include "camera.h"

#include "../math/matrix.h"

void initCamera (Camera* camera) 
{
    camera->position.x = 0.0f;
    camera->position.y = 0.0f;
    camera->position.z = 0.0f;

    camera->facing.x =  0.0f;
    camera->facing.y =  0.0f;
    camera->facing.z =  0.0f;

    camera->worldUp.x = 0.0f;
    camera->worldUp.y = 1.0f;
    camera->worldUp.z = 0.0f;

    camera->forward.x = 0.0f;
    camera->forward.y = 0.0f;
    camera->forward.z = 1.0f;

    camera->speed = 0.4;
    camera->sensitivity = 0.001;

    camera->pitch = 0.0f;
    camera->yaw = 0.0f;
}

void moveCamera (Camera* camera, Vec3 velocity) 
{
    velocity = scaleVector(normalize(velocity), camera->speed);

    camera->position = addVector(camera->position, velocity);

}

void getView (GLFWwindow* window, const Camera camera, float* matrix) 
{
    set_matrix_3d(matrix, window, camera, 45.0f, 100.0f);
}