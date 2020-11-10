#include "Fire.h"
#include "Goomba.h"

CFire::CFire() {

	type = FIRE_TYPE;
	float x_mario, y_mario;
	mario->GetInstance()->GetPosition(x_mario, y_mario);
	SetPosition(x_mario, y_mario);
	AddAnimation(809);
	AddAnimation(810);
	if (mario->GetInstance()->nx > 0)
	{
		vx = FIRE_SPEED;
		vy = -(FIRE_SPEED + 0.2f);
	}
	else
	{
		vx = -FIRE_SPEED;
		vy = -(FIRE_SPEED + 0.2f);
	}
	SetState(FIRE_STATE_NOT_BURN); //set trang thai ban dau la binh`
	dead = false;
	startCounting = GetTickCount();
}
void CFire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIRE_BBOX_WIDTH;
	bottom = y + FIRE_BBOX_HEIGHT;
}

void CFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy 

	vector<LPGAMEOBJECT> coEvents;

	coEvents.clear();

	if (GetTickCount() - startCounting > TIME_FIRE)
	{
		dead = true;
		startCounting = 0;
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (IsCollisionAABB(GetRect(), coObjects->at(i)->GetRect()))
		{
			coEvents.push_back(coObjects->at(i));
		}
	}
	//float cx, cy;
	//CGame::GetInstance()->GetCamPos(cx, cy);
	//if (y > cy + 475) //SCREEN_HEIGHT
	//{
	//	dead = true;
	//}
	if (coEvents.size() == 0) {}
	else
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (coEvents.at(i)->type == GOOMBA_TYPE)
			{
				coEvents.at(i)->dead = true;
			}

		}
	}

}
void CFire::SetState(int state)
{
	CGameObject::SetState(state);

	/*switch (state)
	{
	case FIRE_STATE_NOT_BURN:
		break;
	case FIRE_STATE_BURN:
		break;
	}*/
}
void CFire::Render()
{
	int ani;
	if (state == FIRE_STATE_NOT_BURN)
		ani = FIRE_ANI_NOT_BURN;
	else ani = FIRE_ANI_BURN;

	animations[ani]->Render(x, y);
	RenderBoundingBox();
}
void CFire::VaChamDat(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt);
	vy += FIRE_GRAVITY * dt;
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
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		SetState(FIRE_STATE_BURN);
		

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}