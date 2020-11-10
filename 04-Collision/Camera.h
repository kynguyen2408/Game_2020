#pragma once
#include "Game.h"

#define BEGIN_MAP1 0
#define END_MAP1 1500
#define BEGIN_MAP2 0
#define END_MAP2 3084
#define BEGIN_MAP3 3074
#define END_MAP3 4110
#define BEGIN_MAP4 3074
#define END_MAP4 4110
#define MAP_4_Y 400 
#define BEGIN_MAP5 4095
#define END_MAP5 5642

#define SCREEN_WIDTH 550
#define SCREEN_HEIGHT 475

class CCamera
{

	static CCamera* _instance;
public:
	float x, y;
	float width, height;
	int map;
	static CCamera* GetInstance();
	void Update();
};


