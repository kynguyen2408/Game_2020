#include "Goomba.h"
CGoomba::CGoomba()
{
	type = GOOMBA_TYPE;
	dead = false;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
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

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 800) {
		x = 790; vx = -vx;
	}
}

void CGoomba::Render()
{
	int ani;
	if (vx > 0) ani = GOOMBA_ANI_WALKING_RIGHT;
	else ani = GOOMBA_ANI_WALKING_LEFT;

	if (state == GOOMBA_STATE_DIE) {
		dead = true;
		ani = GOOMBA_ANI_DIE;
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
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING_RIGHT: 
			vx = GOOMBA_WALKING_SPEED;
		case GOOMBA_STATE_WALKING_LEFT:
			vx = -GOOMBA_WALKING_SPEED;
	}
}
