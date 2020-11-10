#include "Ball.h"

CBall::CBall() {

	type = BALL_TYPE;
	float x_aquaman, y_aquaman;
	aquaman->GetInstance()->GetPosition(x_aquaman, y_aquaman);
	SetPosition(x_aquaman, y_aquaman);
	if (aquaman->GetInstance()->vx > 0)
	{
		AddAnimation(918); // dao phai
		vx = BALL_SPEED;
	}
	else
	{
		AddAnimation(917); // dao trai
		vx = -BALL_SPEED;
	}

	dead = false;

}
void CBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BALL_BBOX_WIDTH;
	bottom = y + BALL_BBOX_HEIGHT;
}

void CBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	x += dx;

	// Calculate dx, dy 

	vector<LPGAMEOBJECT> coEvents;

	coEvents.clear();

	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	if (IsCollisionAABB(GetRect(), coObjects->at(i)->GetRect()))
	//	{
	//		coEvents.push_back(coObjects->at(i));
	//	}
	//}
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	if (x > cx + 500 || x < cx) //SCREEN_WIDTH
	{
		dead = true;
	}
	if (coEvents.size() == 0) {}
	else
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (coEvents.at(i)->type == BALL_TYPE)
			{
				coEvents.at(i)->dead = true;
				dead = true;
			}

		}
	}

}

void CBall::Render()
{
	int ani;
	ani = BALL_ANI;
	animations[ani]->Render(x,y);
	RenderBoundingBox();
}
