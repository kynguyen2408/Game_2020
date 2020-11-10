#include "Bat.h"


CBat::CBat()
{
	type = BAT_TYPE;
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
	x += vx * dt;
	y += 1.2 * sin(x*0.03f);


	if (vx < 0 && x < 0) {
		 vx = -vx;
	}

	if (vx > 0 && x > 400) {
		x = 400;
		 vx = -vx;
		 y -= 1.2 * sin(x*0.03f);
	}
}

void CBat::Render()
{
	animations[0]->Render(x, y);
	RenderBoundingBox();
}


void CBat::SetState(int state)
{
}
