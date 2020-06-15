#include "entity.h"

void Player_Init(Player* p)
{
	p->e.posX = 0.0f;
	p->e.posY = 0.0f;
	p->e.posZ = 0.0f;
	p->e.rotX = 0.0f;
	p->e.rotY = 0.0f;
	p->e.rotZ = 0.0f;
	p->e.scale = 0.05f;
}
