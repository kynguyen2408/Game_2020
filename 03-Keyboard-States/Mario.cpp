#include "Mario.h"
#include "Debug.h"
void CMario::Update(DWORD dt)
{
	CGameObject::Update(dt);

	// simple fall down
	vy += MARIO_GRAVITY;
	if (y >= 100) 
	{
		vy = 0; y = 100.0f;
		jumping = false;
		sitting = false;
	}
	
	// simple screen edge collision!!!
	if (vx > 0 && x > 550) x = 550;
	if (vx < 0 && x < 0) x = 0;
}

void CMario::Render()
{
	int ani;
	if (vx == 0)
	{
		if (nx > 0) 
		{
			ani = MARIO_ANI_IDLE_RIGHT;
			if (vy < 0) ani = MARIO_ANI_JUMP_RIGHT;
			else if (ny == -1) ani = MARIO_ANI_SIT_RIGHT;
		}
		else
		{
			ani = MARIO_ANI_IDLE_LEFT;
			if (vy < 0) ani = MARIO_ANI_JUMP_LEFT;
			else if (ny == -1) ani = MARIO_ANI_SIT_LEFT;
		}
	}
	else if (vx > 0)
	{
		if (nx > 0)
		{
			ani = MARIO_ANI_WALKING_RIGHT;
			if (vy != 0) ani = MARIO_ANI_JUMP_RIGHT;
		}
	}
	else
	{
		ani = MARIO_ANI_WALKING_LEFT;
		if (vy != 0) ani = MARIO_ANI_JUMP_LEFT;
	}

	animations[ani]->Render(x, y);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP: 
		if (y == 100)
		{
			vy = -MARIO_JUMP_SPEED_Y;
			ny = 1;
			jumping = true;
		}
		break;
	case MARIO_STATE_SIT:
		sitting = true;
		vx = 0;
		ny = -1;
		break;
	case MARIO_STATE_SIT_LEFT:
		sitting = true;
		nx = -1;
		vx = 0;
		ny = -1;
		break;
	case MARIO_STATE_SIT_RIGHT:
		sitting = true;
		nx = 1;
		vx = 0;
		ny = -1;
		break;
	case MARIO_STATE_IDLE: 
		vx = 0;
		vy = 0;
		ny = 0;
		break;
	}
}

