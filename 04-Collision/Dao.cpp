#include "Dao.h"
#include "Goomba.h"

CDao::CDao() {
	
	type = DAO_TYPE;
	float x_mario, y_mario;
	mario->GetInstance()->GetPosition(x_mario, y_mario);
	SetPosition(x_mario, y_mario);
	if (mario->GetInstance()->nx > 0)
	{
		AddAnimation(803); // dao phai
		vx = DAO_SPEED;
	}
	else 
	{
		AddAnimation(804); // dao trai
		vx = -DAO_SPEED;
	}
	//IsThrowing = false;
	dead = false;

}
void CDao::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DAO_BBOX_WIDTH;
	bottom = y + DAO_BBOX_HEIGHT;
}

void CDao::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	x += dx;
	
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
	float cx,cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	if (x > cx+500 || x < cx) //SCREEN_WIDTH
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

void CDao::Render()
{
	int ani;
	ani = DAO_ANI;
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}
