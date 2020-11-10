#include "Aquaman.h"
#include "Brick.h"
CAquaman::CAquaman()
{
	type = AQUAMAN_TYPE;
	dead = false;
	AddAnimation(911); //AQUAMAN_STAND_LEFT
	AddAnimation(912); //AQUAMAN_STAND_RIGHT);
	AddAnimation(913); //AQUAMAN_WALK_LEFT);
	AddAnimation(914); //AQUAMAN_WALK_RIGHT);
	AddAnimation(915); //AQUAMAN_FIRE_LEFT);
	AddAnimation(916); //AQUAMAN_FIRE_RIGHT);
	startCounting = 0;
	aquaman_x = AQUAMAN_X;
	aquaman_y = AQUAMAN_Y;
	onYard = false;
	
}

void CAquaman::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + AQUAMAN_WIDTH;

	if (state == AQUAMAN_STATE_DIE)
		bottom = y + AQUAMAN_WIDTH;
	else
		bottom = y + AQUAMAN_HEIGHT;
}

void CAquaman::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (vx < 0 && x < 100) {
		x = 100; vx = -vx;
	}

	if (vx > 0 && x > 400) {
		x = 400; vx = -vx;
	}
}

void CAquaman::Render()
{
	int ani;
	if (vx > 0) ani = AQUAMAN_WALK_RIGHT;
	else ani = AQUAMAN_WALK_LEFT;
	if (state == AQUAMAN_FIRE_LEFT) {

		ani = AQUAMAN_FIRE_RIGHT;
		vx = 0;
	}

	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void CAquaman::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case AQUAMAN_STATE_DIE:
			y += AQUAMAN_WIDTH - AQUAMAN_STATE_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case AQUAMAN_WALK_RIGHT:
			vx = AQUAMAN_WALKING_SPEED;
		case AQUAMAN_WALK_LEFT:
			vx = -AQUAMAN_WALKING_SPEED;
		case AQUAMAN_FIRE_LEFT:
			break;
		case AQUAMAN_FIRE_RIGHT:
			break;

	}
}
void CAquaman::VaChamDat(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt);
	coEvents.clear();
	if (onYard && GetTickCount() - startCounting > 100) {
		vy += AQUAMAN_GRAVITY * dt;
		startCounting = 0;
	}
	else
	{
		vy += -AQUAMAN_GRAVITY * dt;
	}
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;



		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick *>(e->obj)) 
			{
				CBrick *brick = dynamic_cast<CBrick *>(e->obj);

				if (e->ny == 1)
				{
					y += dy;
					StartCounting();
				}

			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}


CAquaman * CAquaman::GetInstance()
{
	if (__instance == NULL) __instance = new CAquaman();
	return __instance;
}