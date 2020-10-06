#include "Nen.h"
#include "Roi.h"
#include "debug.h"

Nen::Nen()
{
	type = NEN_TYPE;
	dead = false;
	isDestroy = false;
	isItems = false;
}

void Nen::Render()
{
	int ani;
	if (isDestroy) {
		ani = NEN_ANI_ITEM;
		state = CANDLE_STATE_2;
	}
	else ani = NEN_ANI_BIG;
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Nen::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x;
	if (isDestroy) {
		bottom = y + BRICK_BBOX_HEIGHT;
	}

	else
		bottom = y + BRICK_BBOX_HEIGHT;
//	bottom = y + BRICK_BBOX_HEIGHT;
}

void Nen::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();
	//CalcPotentialCollisions(coObjects, coEvents);

	////
	//// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//// 

	//x += dx;
	//y += dy;

	//if (vx < 0 && x < 0) {
	//	x = 0; vx = -vx;
	//}

	//if (vx > 0 && x > 290) {
	//	x = 290; vx = -vx;
	//}
	//if (coEvents.size() == 0)
	//{
	//	for (UINT i = 0; i < coObjects->size(); i++) {
	//		if (IsCollision(GetRect(), coObjects->at(i)->GetRect())) {
	//			if (coObjects->at(i)->type == ROI_TYPE)
	//				dead = true;
	//		}
	//	}
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

	//		if (dynamic_cast<CRoi *>(e->obj))  
	//		{
	//			SetState(NEN_STATE_DIE);
	//			DebugOut(L"chet");
	//		}
	//	}

	//}
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Nen::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CANDLE_STATE_1: 
		isDestroy = true;
		break;
	case CANDLE_STATE_2:

	case CANDLE_STATE_DIE:
		dead = true;
		isDestroy = false;
		break;
	case CANDLE_STATE_BIG:
	case CANDLE_STATE_SMALL:
		break;
	default:
		break;
	}
}


//void Candle::SetState(int state)
//{
//	GameObject::SetState(state);
//
//	switch (state)
//	{
//	case BIG_CANDLE:
//	case SMALL_CANDLE:
//		break;
//	case CANDLE_DESTROYED:
//		animations[state]->SetAniStartTime(GetTickCount());
//		break;
//	default:
//		break;
//	}
//}
//
//void Candle::GetBoundingBox(float & left, float & top, float & right, float & bottom)
//{
//	left = x;
//	top = y;
//
//	switch (state)
//	{
//	case BIG_CANDLE:
//		right = left + CANDLE_BBOX_WIDTH;
//		bottom = top + CANDLE_BBOX_HEIGHT;
//		break;
//	case SMALL_CANDLE:
//		right = left + SMALL_CANDLE_BBOX_WIDTH;
//		bottom = top + SMALL_CANDLE_BBOX_HEIGHT;
//		break;
//	default:
//		right = left;
//		bottom = top;
//		break;
//	}
//}