#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "Nen.h"
#include "Goomba.h"
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{


	CGameObject::Update(dt);
	vector<LPGAMEOBJECT> coEventsStatic;
	vector<LPCOLLISIONEVENT> coEventsResultStatic;

	coEventsStatic.clear();

	// Collision logic with 
	for (UINT i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);

		if (IsCollisionAABB(GetRect(), coObjects->at(i)->GetRect()))
		{
			coEventsStatic.push_back(coObjects->at(i));
		}
	}


	if (coEventsStatic.size() == 0)
	{

	}
	else
	{
		for (UINT i = 0; i < coEventsStatic.size(); i++)
		{
			if (coEventsStatic.at(i)->type == NEN_TYPE && coEventsStatic.at(i)->state == CANDLE_STATE_2) {
				currentRoi = 1;
				coEventsStatic.at(i)->dead = true;
			}
		}

	}


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
			if (dynamic_cast <CGoomba *> (e->obj))
			{
				CGoomba *goomba = dynamic_cast<CGoomba *> (e->obj);
				if (e->ny != 0 /*&& e->nx < 0*/) //va tram ben tren ben trai
				{

					if (CGameObject::GetState() != MARIO_STATE_INJURED)
					{
						CGameObject::nx = 1;
						vx = -0.1;
						SetState(MARIO_STATE_INJURED);
					}
				}
				//else if (e->ny != 0 && e->nx > 0) //va tram ben tren ben phai
				//{
				//	if (goomba->GetState() != GOOMBA_STATE_DIE)
				//	{
				//		CGameObject::nx = -1;
				//		vx = 0.1;
				//		SetState(MARIO_STATE_INJURED);
				//	}
				//}
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
					if (goomba->GetState() != GOOMBA_STATE_DIE)
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
	/*else if (vx > 0)
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
	}*/
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
	case MARIO_STATE_LAUNCH:
		if (launching == false)
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
			launching = true; //cho phep phong dao
			allowCreateWhip = true;
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
		jumping = false;
		injured = false;

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