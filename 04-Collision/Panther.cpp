#include "Panther.h"
#include "Mario.h"
#include "Brick.h"
#include "debug.h"
CPanther::CPanther()
{
	type = PANTHER_TYPE;
	catalog = CATALOG_ENEMY;

	
	AddAnimation(904); //right
	AddAnimation(903); //left
	AddAnimation(906); //run right
	AddAnimation(905); //run left



	SetState(PANTHER_STATE_IDLE);
	if (mario->GetInstance()->nx > 0)
		nx = -1;
	else nx = 1;
	ny = 0;
	dead = false;
}

void CPanther::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}

void CPanther::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPGAMEOBJECT> coEvents; 
	coEvents.clear();

	float x_mario, y_mario;
	
	mario->GetInstance()->GetPosition(x_mario, y_mario);
	if (abs(x) - abs(x_mario) <= 100)
	{
		SetState(PANTHER_STATE_RUN);
	}

	//quay dau 
	if (vx < 0 && x < MovePosX) {
		dead = true;

	}

	if (vx > 0 && x > MovePosDesX) {
		dead = true;
	}
	
}

void CPanther::Render()
{
	int ani;
	if (vx == 0)
	{
		if (nx > 0)
		{
			ani = PANTHER_ANI_IDLE_RIGHT;
		}
		else ani = PANTHER_ANI_IDLE_LEFT;
	}
	else
	{
		if (nx > 0) ani = PANTHER_ANI_RUN_RIGHT;
		else ani = PANTHER_ANI_RUN_LEFT;
	}

	if (state == PANTHER_STATE_DIE) {
		dead = true;
	}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void CPanther::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PANTHER_STATE_IDLE:
		vx = 0;
		vy = 0;
		ny = 0;
		break;
	case PANTHER_STATE_RUN:
		if (nx == 1)
		{
			vx = PANTHER_RUN_SPEED;
		}
		else if(nx == -1)
		{
			vx = -PANTHER_RUN_SPEED;
		} 
		break;
	case PANTHER_STATE_JUMP:
		ny = -1;
		vy = -0.4;
		jumping = true;
		break;
	case PANTHER_STATE_DIE:
		vx = 0;
		vy = 0;
		ny = 0;
		break;
	}
}
void CPanther::VaChamDat(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt);
	vy += PANTHER_GRAVITY * dt;
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
		float min_tx, min_ty, e_nx, e_ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, e_nx, e_ny);
		

		if (e_ny == 1 || e_nx != 0)
		{
			x += dx;
			y += dy;
			

		}
		else
		{
			x += min_tx * dx + e_nx * 0.4f;		
			y += min_ty * dy + e_ny * 0.4f;
		
			if (jumping == true)
			{
				if (nx == 1) nx = -1;
				else if (nx == -1) nx = 1;
				jumping = false;
				SetState(PANTHER_STATE_RUN);
			}
		}
		if (e_ny == -1) vy = 0;

	
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast <CBrick*> (e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*> (e->obj);
				float w, h;
				brick->GetWH(w, h);
				if (jumping==false)
				{
					if (x - brick->x <= 1 || (brick->x + w) - (x + 50) <= 1)
					{

						SetState(PANTHER_STATE_JUMP);
					}
				}
				
			}
		}

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}