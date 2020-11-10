#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "Nen.h"
#include "Stairs.h"
#include "Goomba.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{


	CGameObject::Update(dt);
	vector<LPGAMEOBJECT> coEventsStatic;
	vector<LPCOLLISIONEVENT> coEventsResultStatic;

	coEventsStatic.clear();

	// va cham voi vat dung yen 
	for (UINT i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);
		// kiem tra 2 hinh de nhau hay k 
		if (IsCollisionAABB(GetRect(), coObjects->at(i)->GetRect()))
		{
			coEventsStatic.push_back(coObjects->at(i));
		}
	}

	// neu co va cham 
	//k vo va cham 
	if (coEventsStatic.size() == 0)
	{

	}
	//co va cham dung yen
	else
	{
		for (UINT i = 0; i < coEventsStatic.size(); i++)
		{
			if (coEventsStatic.at(i)->type == NEN_TYPE && coEventsStatic.at(i)->state == CANDLE_STATE_2) {
				//currentRoi = 1;
				coEventsStatic.at(i)->dead = true;
			}
			if (coEventsStatic.at(i)->type == STAIRS_TYPE)
			{

			}
		}

	}

	//xet vat di chuyen 
	vector<LPCOLLISIONEVENT> coEventMoving;
	vector<LPCOLLISIONEVENT> coEventsResultMoving;

	coEventMoving.clear();
	CalcPotentialCollisions(coObjects, coEventMoving);
	

	if (coEventMoving.size() == 0)
	{
		
	}
	else
	{
	
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEventMoving, coEventsResultMoving, min_tx, min_ty, nx, ny);
		for (UINT i = 0; i < coEventsResultMoving.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResultMoving[i];
			if (e->obj->catalog==CATALOG_ENEMY)
			{
		
				if (e->ny != 0) //va tram ben tren ben trai
				{
					if (CGameObject::GetState() != MARIO_STATE_INJURED)
					{
						CGameObject::nx = -CGameObject::nx;
						if (vx <= 0)
							vx = -0.1;
						else
							vx = 0.1;
						SetState(MARIO_STATE_INJURED);
					}
				}
				else if (e->nx < 0) //va tram vang ben trai
				{
					if (CGameObject::GetState() != MARIO_STATE_INJURED)
					{
						CGameObject::nx = 1;
						vx = -0.1;
						SetState(MARIO_STATE_INJURED);

					}
				}
				else if (e->nx > 0) //va tram vang ben phai
				{
					if (e->obj->GetState() != MARIO_STATE_INJURED)
					{
						CGameObject::nx = -1;
						vx = 0.1;
						SetState(MARIO_STATE_INJURED);

					}
				}
				/*else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
								SetState(MARIO_STATE_DIE);
						}
					}
				}*/
			}
		}
	}
	for (UINT i = 0; i < coEventMoving.size(); i++) delete coEventMoving[i];

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
	else
	{
		if (nx > 0)
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
		
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	if (hitting && animations[ani]->isLastFrame == true)
	{
		hitting = false;
		if (ani == MARIO_ANI_HIT_RIGHT)
			ani = MARIO_ANI_IDLE_RIGHT;
		else if (ani == MARIO_ANI_HIT_LEFT)
			ani = MARIO_ANI_IDLE_LEFT;
		else if (ani == MARIO_ANI_HIT_SIT_RIGHT)
			ani = MARIO_ANI_SIT_RIGHT;
		else if (ani == MARIO_ANI_HIT_SIT_LEFT)
			ani = MARIO_ANI_SIT_LEFT;
	}
	animations[ani]->Render(x, y, alpha);
	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		sitting = false;
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		ny = 0;
		break;
	case MARIO_STATE_WALKING_LEFT:
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
	case MARIO_STATE_THROW:
		if (throwing == false && allowCreateSecondWeapon == true )
		{
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
			if(secondWeapon != 0) 
				throwing = true; //danh vu khi phu.
			else
				allowCreateWhip = true; //danh roi
			hitting = true;
		}
		break;
	case MARIO_STATE_INJURED:
		if (injured == false)
		{
			vy = -0.5;
			injured = true;
		}
		break;
	case MARIO_STATE_IDLE:
		sitting = false;
		vx = 0;
		ny = 0;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	
	left = x;
	if (sitting || (jumping && !hitting))
		top = y + 17;
	else top = y;
	right = x + 32;
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

	
	
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		// block 
		jumping = false;
		injured = false;

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;


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