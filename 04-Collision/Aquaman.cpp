#include "Aquaman.h"
#include "Brick.h"
#include "Ball.h"
CAquaman::CAquaman()
{
	type = AQUAMAN_TYPE;
	catalog = CATALOG_ENEMY;
	dead = false;
	AddAnimation(911); //AQUAMAN_STAND_LEFT
	AddAnimation(912); //AQUAMAN_STAND_RIGHT);
	AddAnimation(913); //AQUAMAN_WALK_LEFT);
	AddAnimation(914); //AQUAMAN_WALK_RIGHT);
	AddAnimation(915); //AQUAMAN_FIRE_LEFT);
	AddAnimation(916); //AQUAMAN_FIRE_RIGHT);
	startCounting = 0;
	vy = -0.9f;
	onYard = false;
	isFire = false;
	isSitting = false;
	

	
}

void CAquaman::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + width;

	if (state == AQUAMAN_STATE_DIE)
		bottom = y + width;
	else
		bottom = y + height;
}

void CAquaman::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	//CGameObject::Update(dt, coObjects);

	//x += dx;
	//y += dy;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (vx < 0 && x < aquaman_move_left) {
		vx = -vx;
		SetState(AQUAMAN_STATE_SIT_RIGHT);
		StartCounting();
	}

	if (vx > 0 && x > aquaman_move_right ) {
		vx = -vx;
		SetState(AQUAMAN_STATE_SIT_LEFT);
		StartCounting();
	}
	if (state == AQUAMAN_STATE_SIT_RIGHT) {
		if (GetTickCount() - startCounting > 100) {
			startCounting = 0;
			SetState(AQUAMAN_STATE_FIRE_RIGHT);
		}

	}
	else if (state == AQUAMAN_STATE_SIT_LEFT) {
		if (GetTickCount() - startCounting > 100) {
			startCounting = 0;
			SetState(AQUAMAN_STATE_FIRE_LEFT);
		}

	}
	/*float x, y;
	GetPosition(x, y);
	if (x == 400 || x == 100) {
		ball = new CBall();
		objects.push_back(ball);
		aquaman->SetState(AQUAMAN_FIRE_LEFT);
		aquaman->animations[0]->isLastFrame = false;
	}*/
	
}

void CAquaman::Render()
{
	int ani;
	if (vx > 0) ani = AQUAMAN_WALK_RIGHT;
	else if (vx < 0) ani = AQUAMAN_WALK_LEFT;
	else {
		if (nx == 1) {
			ani = AQUAMAN_FIRE_LEFT;
		}
		else if (nx == -1) ani = AQUAMAN_FIRE_RIGHT;
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
		case AQUAMAN_STATE_SIT_RIGHT:
			vx = 0;
			nx = 1;
			isSitting = true;
			break;
		case AQUAMAN_STATE_SIT_LEFT:
			vx = 0;
			nx = -1;
			isSitting = true;
			break;
		case AQUAMAN_STATE_FIRE_RIGHT:
			isFire = true;
			break;
		case AQUAMAN_STATE_FIRE_LEFT:
			isFire = true;
			break;
		case AQUAMAN_WALK_RIGHT:
			isSitting = false;
			vx = AQUAMAN_WALKING_SPEED;
			nx = 1;
			break;
		case AQUAMAN_WALK_LEFT:
			isSitting = false;
			vx = -AQUAMAN_WALKING_SPEED;
			nx = -1;
			break;



	}
}
void CAquaman::VaChamDat(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGameObject::Update(dt);
	coEvents.clear();
	vy += AQUAMAN_GRAVITY * dt;
		
	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, e_nx, e_ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, e_nx, e_ny);
		
		if (e_ny == -1 && e_nx == 0)
		{
 			x += min_tx * dx + e_nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + e_ny * 0.4f;
			vy = 0;
			/*SetState(AQUAMAN_WALK_RIGHT);*/
			
		}
		else
		{ 
			x += dx;
			y += dy;
		}
	
		/*for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->type == BRICK_TYPE)
			{
				CBrick *brick = dynamic_cast<CBrick *>(e->obj);

				if (e_ny == -1)
				{
					vy = 0;
					
				}

			}
		}*/
		// clean up collision events
		
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


