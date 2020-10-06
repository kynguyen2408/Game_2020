#include "Ghost.h"
void Ghost::Update(DWORD dt)
{
	CGameObject::Update(dt);

	x += vx * dt;
	if ((vx > 0 && x > 460) || (x < 0 && vx < 0)) vx = -vx;
}

void Ghost::Render()
{
	LPANIMATION ani;
	//if (vx>0) ani = animations[0]; else ani = animations[1];

	if (vx > 0) ani = CAnimations::GetInstance()->Get(601);
	else ani = CAnimations::GetInstance()->Get(600);

	//ani = animations[0];
	ani->Render(x, y);
}