#pragma once
#include "type.h"

typedef struct Camera
{
	vec3f position;
	vec3f front;
	vec3f up;
	f32 yaw;
	f32 pitch;
	f32 lastX, lastY;
} Camera;

void Camera_Init(Camera* camera);
