#pragma once

#include "GameObject.h"
#include "Mario.h"

#include "Goomba.h"
#define ROI_BBOX_WIDTH 60
#define ROI_BBOX_HEIGHT 20
#define ROI_BBOX_HEIGHT_DIE 9
#define ROI_STATE_DIE	10000
#define ROI_ANI 0
#define ROI_ANI1 1
#define ROI_ANI2 2



#define ROI_TYPE		111
class CRoi: public CGameObject
{
	bool ishitting;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

public:
	CMario* mario;
	CRoi();
	void SetState(int state);

};

