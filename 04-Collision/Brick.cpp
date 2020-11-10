#include "Brick.h"

CBrick::CBrick()
{
	type = BRICK_TYPE;
}

void CBrick::Render()
{
	
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}