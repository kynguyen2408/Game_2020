#include "Simon.h"

void Simon::Update(DWORD dt)
{
	CGameObject::Update(dt);

	x += vx*dt;
	if ((vx>0 && x > 460) || (x < 0 && vx<0)) vx=-vx;
	y += vy*dt;
	if ((vy > 0 && y > 300) || (y < 0 && vy < 0)) vy = -vy;
}

void Simon::Render()
{
	LPANIMATION ani;
	//if (vx>0) ani = animations[0]; else ani = animations[1];

	if (vx > 0) ani = CAnimations::GetInstance()->Get(500);
	else ani = CAnimations::GetInstance()->Get(501);

	//ani = animations[0];
	ani->Render(x, y);
}