#include "Bat.h"


CBat::CBat()
{
	type = BAT_TYPE;
	catalog = CATALOG_ENEMY;
	AddAnimation(909); // ani left
	AddAnimation(910); // ani right
	width = BAT_WIDTH;
	height = BAT_HEIGHT;
	nx = 1;
}

void CBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx = nx > 0 ? 0.1f : -0.1f;
	ani = nx > 0 ? 1 : 0;
	x += vx * dt;
	y += 1.1 * sin(x*0.03f);


	if (vx < 0 && x < MovePosX) {
		nx = -nx;
	}

	if (vx > 0 && x > MovePosDesX) {
		nx = -nx;
		
	}

	if (y < 0) 
		y = 200;
}

void CBat::Render()
{
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}


void CBat::SetState(int state)
{
}
