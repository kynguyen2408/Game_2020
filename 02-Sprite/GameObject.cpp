#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = 0.07f;
	vy = 0.07f;
}

void CGameObject::Update(DWORD dt)
{
	
}

void CGameObject::Render()
{
	
}

CGameObject::~CGameObject()
{
}