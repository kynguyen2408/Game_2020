#pragma once
#include "GameObject.h"

#define MARIO_X					50 
#define MARIO_Y					250
#define MARIO_WALKING_SPEED		0.1f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_SIT				400
#define MARIO_STATE_SIT_LEFT		500
#define MARIO_STATE_SIT_RIGHT		600
#define MARIO_STATE_HIT				700
#define MARIO_STATE_LAUNCH			800
#define MARIO_STATE_THROW			900
#define MARIO_STATE_BURN			1000
#define MARIO_STATE_INJURED			1100
#define MARIO_STATE_DIE				1200

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3
#define MARIO_ANI_JUMP_RIGHT		4
#define MARIO_ANI_JUMP_LEFT			5
#define MARIO_ANI_SIT_RIGHT			6
#define MARIO_ANI_SIT_LEFT			7
#define MARIO_ANI_HIT_RIGHT			8
#define MARIO_ANI_HIT_LEFT			9
#define MARIO_ANI_HIT_SIT_RIGHT		10
#define MARIO_ANI_HIT_SIT_LEFT		11

#define MARIO_ANI_DIE				16

#define	MARIO_TYPE 11

#define MARIO_BIG_BBOX_WIDTH  40
#define MARIO_BIG_BBOX_HEIGHT 60


#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
private:
	static CMario* _instance;
	int level;
	int untouchable;
	DWORD untouchable_start;
	static CMario * __instance;

public: 
	boolean jumping = false;
	boolean sitting = false;
	boolean hitting = false;
	int secondWeapon = 0;
	boolean throwing = false;
	boolean injured = false;
	boolean allowCreateWhip = false;
	boolean isBringItems = false;
	boolean coRiu = false;
	int whipType;	// type of whip 1,2,3 
	int bullet;
	int point;
	int alpha, beta;		//for render
	int health;
	int life;
	int bossHealth;
	bool freeze;
	int numWeapon;
	boolean allowCreateSecondWeapon = true;
	int currentRoi;

	float mario_x, mario_y;
	CMario() : CGameObject()
	{
		untouchable = 0;
		mario_x = MARIO_X;
		mario_y = MARIO_Y;
		whipType = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	static CMario* GetInstance();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void vaChamTuong(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	bool CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem);
	int GetCurrentWeapons() { return whipType; }
};