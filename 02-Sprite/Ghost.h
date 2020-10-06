#pragma once
#include "GameObject.h"

class Ghost : public CGameObject
{
public:

	void Update(DWORD dt);
	void Render();
};

