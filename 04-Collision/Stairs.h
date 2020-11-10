#pragma once
#include "GameObject.h"

#define STAIRS_BBOX_WIDTH 30
#define STAIRS_BBOX_HEIGHT 20

#define STAIRS_TYPE		115

class CStairs : public CGameObject
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
public:
	CStairs() ;
	~CStairs();
};

