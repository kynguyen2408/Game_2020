#include "Ghost.h"
CGhost::CGhost()
{
	vx = GHOST_WALKING_SPEED;
}
void CGhost::Update(DWORD dt)
{
	CGameObject::Update(dt);
	x += vx * dt;
	if ((vx > 0 && x > 550) || (x < 0 && vx < 0)) vx = -vx;
}
void CGhost::Render()
{
	LPANIMATION ani;
	//if (vx>0) ani = animations[0]; else ani = animations[1];

	if (vx > 0) ani = CAnimations::GetInstance()->Get(801);
	else ani = CAnimations::GetInstance()->Get(800);

	//ani = animations[0];
	ani->Render(x, y);
}
