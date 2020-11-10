#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  2000
#define BRICK_BBOX_HEIGHT 16

#define BRICK_WIDTH  200
#define BRICK_HEIGHT 16

#define BRICK_TYPE 14

class CBrick : public CGameObject
{
public:
	CBrick();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

};