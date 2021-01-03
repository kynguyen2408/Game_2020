#include "Ball.h"

CBall::CBall() {}
CBall::CBall(float x, float y,int nx) {
	type = BALL_TYPE;
	catalog = CATALOG_ENEMY;
	AddAnimation(918); // dao phai
	AddAnimation(917); // dao trai
	SetPosition(x, y);
	vx = nx * 0.2;
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
	if (vx > 0) {
		ani = BALL_ANI_RIGHT;
	}
	else ani = BALL_ANI_LEFT;
	animations[ani]->Render(x,y);
	RenderBoundingBox();
}

void CBall::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case BALL_STATE_DIE:
		break;
	case BALL_STATE_LEFT:
		vx = -BALL_SPEED;
		break;
	case BALL_STATE_RIGHT:
		vx = BALL_SPEED;
		break;
	}
}
