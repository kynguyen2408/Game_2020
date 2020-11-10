#include "Riu.h"
#include "Goomba.h"

CRiu::CRiu() {

	type = RIU_TYPE;
	
	float x_mario, y_mario;
	mario->GetInstance()->GetPosition(x_mario, y_mario);
	SetPosition(x_mario, y_mario);
	if (mario->GetInstance()->nx > 0)
	{
		AddAnimation(807);
		vx = RIU_SPEED;
		vy = -(RIU_SPEED + 0.2f);
	}
	else
	{
		AddAnimation(4001);
		vx = -RIU_SPEED;
		vy = -(RIU_SPEED + 0.2f);
	}
	//IsThrowing = false;
	dead = false;

}
void CRiu::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + RIU_BBOX_WIDTH;
	bottom = y + RIU_BBOX_HEIGHT;
}

void CRiu::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += RIU_GRAVITY * dt;

	x += dx;
	y += dy;

	// Calculate dx, dy 

	vector<LPGAMEOBJECT> coEvents;

	coEvents.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (IsCollisionAABB(GetRect(), coObjects->at(i)->GetRect()))
		{
			coEvents.push_back(coObjects->at(i));
		}
	}
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	if (y > cy + 475) //SCREEN_HEIGHT
	{
		dead = true;
	}
	if (coEvents.size() == 0) {}
	else
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (coEvents.at(i)->catalog == CATALOG_ENEMY)
			{
				coEvents.at(i)->dead = true;
				dead = true;
			}

		}
	}

}

void CRiu::Render()
{
	int ani;
	ani = RIU_ANI;
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}
