#pragma once
#include "GameObject.h"

#define STAIRS_BBOX_WIDTH 50
#define STAIRS_BBOX_HEIGHT 20

#define STAIRS_RIGHT_UP		116
#define STAIRS_RIGHT_DOWN	117
#define STAIRS_LEFT_UP		118
#define STAIRS_LEFT_DOWN	119

class CStairs : public CGameObject
{
	//int type_stairs;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
public:
	CStairs();
	CStairs(int t);
	~CStairs();
};

