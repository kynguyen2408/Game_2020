#include "Roi.h"

#include "Goomba.h"

#include "Nen.h"
#include "Mario.h"

CRoi::CRoi() {
	type = ROI_TYPE;
	ishitting = false;
	//IsThrowing = false;
	dead = false;

	if (mario->GetInstance()->nx > 0)
	{
		AddAnimation(603); // roi phai
		AddAnimation(604); // roi phai 2
		AddAnimation(605); // roi phai 3
	}
	else
	{
		AddAnimation(600); // roi trai
		AddAnimation(601); // roi trai 2
		AddAnimation(602); // roi trai 3
	}
	animations[mario->GetInstance()->whipType]->isLastFrame = false;
	animations[mario->GetInstance()->whipType]->currentFrame = -1;

}
void CRoi::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
	if (mario->GetInstance()->whipType == 0) {
		left = x + 30;
		top = y + 20;
		right = x + ROI_BBOX_WIDTH + 70;
		bottom = y + ROI_BBOX_HEIGHT + 12;
	}
	else if (mario->GetInstance()->whipType == 1) {
		left = x + 50;
		top = y + 4;
		right = x + ROI_BBOX_WIDTH + 100;
		bottom = y + ROI_BBOX_HEIGHT + 4;
	}
	else if (mario->GetInstance()->whipType == 2) {
		left = x + 50;
		top = y + 4;
		right = x + ROI_BBOX_WIDTH + 100;
		bottom = y + ROI_BBOX_HEIGHT + 4;
	}

	
}

void CRoi::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	float x_mario, y_mario;
	float y_roi = 0;
	if (mario->GetInstance()->ny < 0) y_roi = 12;

	mario->GetInstance()->GetPosition(x_mario, y_mario);
	if (mario->GetInstance()->nx > 0)
		SetPosition(x_mario - 30, y_mario - 4 + y_roi);
	else SetPosition(x_mario - 90, y_mario - 4 + y_roi);
	// Calculate dx, dy 

	vector<LPGAMEOBJECT> coEvents;

	coEvents.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (IsCollisionAABB(GetRect(), coObjects->at(i)->GetRect()))
		{
			coEvents.push_back(coObjects->at(i));
		}
	}
	if (coEvents.size() == 0) {}
	else
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (coEvents.at(i)->type == GOOMBA_TYPE)
				coEvents.at(i)->dead = true;
			else if (coEvents.at(i)->type == NEN_TYPE) {
				coEvents.at(i)->SetState(CANDLE_STATE_BUMP);
			}
		}
	}
}

void CRoi::Render()
{
	int ani;

	if (nx == 1) {
		ani = ROI_ANI;
	}
	ani = mario->GetInstance()->whipType;
	if (animations[ani]->currentFrame == 3) {
		dead = true;
	}
	


	animations[ani]->Render(x, y);

	if(animations[ani]->currentFrame == 3)
	{
		RenderBoundingBox();
	}
	

}

void CRoi::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ROI_ANI:
		nx = 1;
		break;
	}
}