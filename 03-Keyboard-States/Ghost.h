#pragma once
#include "GameObject.h"

#define GHOST_WALKING_SPEED		0.05f

#define GHOST_STATE_WALKING_RIGHT	100
#define GHOST_STATE_WALKING_LEFT	200

#define GHOST_ANI_WALKING_RIGHT		1
#define GHOST_ANI_WALKING_LEFT		2


class CGhost: public CGameObject
{
public:
	CGhost();
	void Update(DWORD dt);
	void Render();
};

