#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "Nen.h"
#include "Goomba.h"


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Simple fall down

	// Calculate dx, dy 

	CGameObject::Update(dt);
	vector<LPGAMEOBJECT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	// No collision occured, proceed normally


	//float min_tx, min_ty, nx = 0, ny;

	//FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);


	// Collision logic with 
	for (UINT i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);

		if (IsCollisionAABB(GetRect(), coObjects->at(i)->GetRect()))
		{
			coEvents.push_back(coObjects->at(i));
		}
	}
	if (coEvents.size() == 0) {}
	else
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (coEvents.at(i)->type == NEN_TYPE && coEvents.at(i)->state == CANDLE_STATE_2) {
				currentRoi = 1;
				coEvents.at(i)->dead = true;
			}
			//if (coEvents.at(i)->type == GOOMBA_TYPE) {
			//	coEvents.at(i)->dead = true;
			//}


		}
	}

}

void CMario::Render()
{
	int ani;
	if (vx == 0)
	{
		if (nx > 0)
		{
			ani = MARIO_ANI_IDLE_RIGHT;
			if (ny > 0)
			{
				if (hitting == true) ani = MARIO_ANI_HIT_RIGHT;
				else ani = MARIO_ANI_JUMP_RIGHT;
			}
			else if (ny == -1)
			{
				if (hitting == true) ani = MARIO_ANI_HIT_SIT_RIGHT;
				else ani = MARIO_ANI_SIT_RIGHT;
			}
			else if (hitting == true ) ani = MARIO_ANI_HIT_RIGHT;
		}
		else
		{
			ani = MARIO_ANI_IDLE_LEFT;
			if (ny > 0) 
			{
				if (hitting == true) ani = MARIO_ANI_HIT_LEFT;
				else ani = MARIO_ANI_JUMP_LEFT;
			}
			else if (ny == -1)
			{
				if (hitting == true) ani = MARIO_ANI_HIT_SIT_LEFT;
				else ani = MARIO_ANI_SIT_LEFT;
			}
			else if (hitting == true) ani = MARIO_ANI_HIT_LEFT;
		}
	}
	else if (vx > 0)
	{
			ani = MARIO_ANI_WALKING_RIGHT;
			if (ny > 0)
			{
				if (hitting == true) ani = MARIO_ANI_HIT_RIGHT;
				else ani = MARIO_ANI_JUMP_RIGHT;
			}
			else if (hitting == true)
			{	
				ani = MARIO_ANI_HIT_RIGHT;
			}
	}
	else
	{
		ani = MARIO_ANI_WALKING_LEFT;
		if (ny > 0)
		{
			if (hitting == true) ani = MARIO_ANI_HIT_LEFT;
			else ani = MARIO_ANI_JUMP_LEFT;
		}
		else if (hitting == true) ani = MARIO_ANI_HIT_LEFT;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	if (animations[ani]->isLastFrame == true)
		hitting = false;
	animations[ani]->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (sitting)
		{
			y = y - 20;
		}
		sitting = false;
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		ny = 0;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (sitting)
		{
			y = y - 20;
		}
		sitting = false;
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		ny = 0;
		break;
	case MARIO_STATE_JUMP:
		if (jumping == false)
		{
			vy = -MARIO_JUMP_SPEED_Y;
			ny = 1;
			jumping = true;
		}
		break;
	case MARIO_STATE_SIT: 
		vx = 0;
		ny = -1;
		sitting = true;
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
	case MARIO_STATE_HIT:
		if (nx > 0)
		{
			if (ny < 0)
			{
				animations[MARIO_ANI_HIT_SIT_RIGHT]->isLastFrame = false;
				animations[MARIO_ANI_HIT_SIT_RIGHT]->currentFrame = -1;
			}

			else
			{
				animations[MARIO_ANI_HIT_RIGHT]->isLastFrame = false;
				animations[MARIO_ANI_HIT_RIGHT]->currentFrame = -1;
				if (ny == 0) vx = 0;
				
			}
		}
		else
		{
			if (ny < 0)
			{
				animations[MARIO_ANI_HIT_SIT_LEFT]->isLastFrame = false;
				animations[MARIO_ANI_HIT_SIT_LEFT]->currentFrame = -1;
			}
			else
			{
				animations[MARIO_ANI_HIT_LEFT]->isLastFrame = false;
				animations[MARIO_ANI_HIT_LEFT]->currentFrame = -1;
				if (ny == 0) vx = 0;
			}
		}
		
		allowCreateWhip = true;
		hitting = true;
		break;
	case MARIO_STATE_IDLE:
		if (sitting)
		{
			y = y -20;
		}
		sitting = false;
		vx = 0;
		ny = 0;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	
	left = x;
	top = y;
	right = x + 40;
	if (sitting)
		bottom = y + 40;
	else 
		bottom = y + 60;
	
}

void CMario::vaChamTuong(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt);
	vy += MARIO_GRAVITY * dt;
	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	// No collision occured, proceed normally

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		// block 
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//jumping = false;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

bool CMario::CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem)
{

	for (UINT i = 0; i < listItem->size(); i++)
	{
		if (IsCollisionAABB(GetRect(), listItem->at(i)->GetRect()))
		{
			int idItem = listItem->at(i)->GetState(); // kiem tra xem tai day item do co id la gi ? 

			switch (idItem)
			{
			//case HOLY_WATER:
			//	SetCurrentWeapons(idItem);
			}
			return true;
		}
	}
}

CMario * CMario::GetInstance()
{
	if (__instance == NULL) __instance = new CMario();
	return __instance;
}