#include "Goomba.h"
CGoomba::CGoomba()
{
	type = GOOMBA_TYPE;
	catalog = CATALOG_ENEMY;
	dead = false;
	AddAnimation(902);
	AddAnimation(901);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < MovePosX) {
		vx = -vx;
		//x = MovePosX;
	}

	if (vx > 0 && x > MovePosDesX) {
		vx = -vx;
		//x = MovePosDesX;
	}
}

void CGoomba::Render()
{
	int ani;
	if (vx > 0) ani = GOOMBA_ANI_WALKING_RIGHT;
	else ani = GOOMBA_ANI_WALKING_LEFT;

	if (state == GOOMBA_STATE_DIE) {
		dead = true;
	}

	animations[ani]->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING_RIGHT: 
			vx = GOOMBA_WALKING_SPEED;
		case GOOMBA_STATE_WALKING_LEFT:
			vx = -GOOMBA_WALKING_SPEED;
	}
}
