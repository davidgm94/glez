#pragma once
#include "type.h"

typedef struct Entity
{
	f32 posX, posY, posZ;
	f32 rotX, rotY, rotZ;
	f32 scale;
} Entity;

typedef struct Player
{
	Entity e;
	f32 currentSpeed;
	f32 currentTurnSpeed;
} Player;

void Player_Init(Player* p);
