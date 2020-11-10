#pragma once
#pragma once
#include "GameObject.h"
#include "Mario.h"

#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 32
#define NEN_TYPE		3

#define CANDLE_STATE_BIG			300
#define CANDLE_STATE_SMALL			400
#define CANDLE_STATE_DIE			500
#define CANDLE_STATE_ITEMS_WHIP		600
#define CANDLE_STATE_ITEMS_HEART	900
#define CANDLE_STATE_ITEMS_HEART_MALL 800
#define CANDLE_STATE_BUMP			1000
#define CANDLE_STATE_ITEMS_AXE		1100


#define NEN_ANI 0
#define NEN_ANI_ITEM_AXE 1
#define NEN_ANI_ITEM_BIG_HEART 2
#define NEN_ANI_ITEM_DOUBLESHOT 3 
#define NEN_ANI_ITEM_HOLLYWATER 4
#define NEN_ANI_ITEM_INVINCIBILITY 5
#define NEN_ANI_ITEM_KNIFE 6
#define NEN_ANI_ITEM_MONEY_BLUE 7
#define NEN_ANI_ITEM_MONEY_RED 8 
#define NEN_ANI_ITEM_WHITE 9
#define NEN_ANI_ITEM_ROAST 10 
#define NEN_ANI_ITEM_ROSARY 11 
#define NEN_ANI_ITEM_SMALL_HEART 12 
#define NEN_ANI_ITEM_STOPWATCH 13
#define NEN_ANI_ITEM_WHIP 14
#define NEN_ANI_NEN_NHO 15



class Nen : public CGameObject
{
public:
	bool isDestroy;
	bool isItems;
	bool candleBig;
	int typeItems;
	int ani;
	CMario* mario;
	Nen();
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void SetState(int state);
	void setTypeItems(int typeItems);
	int getTypeItems();
};

