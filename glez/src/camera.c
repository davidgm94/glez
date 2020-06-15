#include "camera.h"

void Camera_Init(Camera* camera)
{
	camera->position = VEC3(0.0f, 0.0f, 3.0f);
	camera->front = VEC3(0.0f, 0.0f, -1.0f);
	camera->up = VEC3(0.01f, 1.0f, 0.0f);

	camera->yaw = -90.0f;
	camera->pitch = 0.0f;
	camera->lastX = 1024 / 2.0f;
	camera->lastY = 576 / 2.0f;
}
