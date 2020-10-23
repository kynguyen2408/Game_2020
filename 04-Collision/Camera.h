#pragma once
#include "Game.h"

class CCamera
{

	static CCamera* _instance;
public:
	float x, y;
	float width, height;

	static CCamera* GetInstance();
	void Update();
};


