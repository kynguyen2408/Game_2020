#include "Nen.h"
#include "Roi.h"
#include "debug.h"

Nen::Nen()
{
	candleBig = true;
	type = NEN_TYPE;
	dead = false;
	isDestroy = false;
	isItems = false;
	vy = 0;
	AddAnimation(301); // nen to dung yen
	AddAnimation(806); // axe
	AddAnimation(802); // big heart
	AddAnimation(815); // double shot
	AddAnimation(808); // holly water
	AddAnimation(814); // invincibility
	AddAnimation(803); // knife
	AddAnimation(811); // money blue
	AddAnimation(812); // money red
	AddAnimation(813); // money white
	AddAnimation(818); // roast
	AddAnimation(817); // rosary
	AddAnimation(805); // small heart
	AddAnimation(816); // stop watch
	AddAnimation(801); // whip
	AddAnimation(302); // nen nho

}

void Nen::Render()
{

	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Nen::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + width;
	if (isDestroy) {
		bottom = y + width;
	}

	else
		bottom = y + height;
}

void Nen::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroy) {
		switch (typeItems)
		{
		case NEN_ANI_ITEM_WHIP:
			ani = NEN_ANI_ITEM_WHIP;
			if (mario->GetInstance()->GetCurrentWeapons() == 0) {
				DebugOut(L"whip1");
				state = CANDLE_STATE_ITEMS_WHIP2;
			}
			else if (mario->GetInstance()->GetCurrentWeapons() == 1) {
				DebugOut(L"whip2");
				state = CANDLE_STATE_ITEMS_WHIP3;
			}
			vy = 0.02;
			break;
		case NEN_ANI_ITEM_BIG_HEART:
			ani = NEN_ANI_ITEM_BIG_HEART;
			state = CANDLE_STATE_ITEMS_HEART;
			vy = 0.02;
			break;
		case NEN_ANI_ITEM_AXE: 
			ani = NEN_ANI_ITEM_AXE;
			state = CANDLE_STATE_ITEMS_AXE;
			vy = 0.02;
			break;
		}
	}
	else if (candleBig == true) {
		ani = NEN_ANI;
	}
	else if (candleBig == false) {
		ani = NEN_ANI_NEN_NHO;
	}
	else
		ani = NEN_ANI;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt);
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

void Nen::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CANDLE_STATE_BUMP: // roi va cham nen -> candle sinh ra items
		isDestroy = true;
		break;
	case CANDLE_STATE_DIE:
		dead = true;
		isDestroy = false;
		break;
	case CANDLE_STATE_ITEMS_WHIP2:
	case CANDLE_STATE_ITEMS_WHIP3:
	case CANDLE_STATE_ITEMS_HEART:
	case CANDLE_STATE_ITEMS_AXE:
	case CANDLE_STATE_BIG:
		candleBig = true; 
		break;
	case CANDLE_STATE_SMALL:
		candleBig = false;
		break;
		break;
	default:
		break;
	}
}

void Nen::setTypeItems(int typeItems)
{
	this->typeItems = typeItems;
}

int Nen::getTypeItems()
{
	return this->typeItems;
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