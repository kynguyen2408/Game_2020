#include "Roi.h"

#include "Goomba.h"

#include "Nen.h"
#include "Mario.h"
CRoi::CRoi() {
	type = ROI_TYPE;
	//IsThrowing = false;
	dead = false;

}
void CRoi::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (mario->GetInstance()->currentRoi == 0) {
		left = x + 50;
		top = y + 4;
		right = x + ROI_BBOX_WIDTH + 50;
		bottom = y + ROI_BBOX_HEIGHT + 4;
	}
	else if (mario->GetInstance()->currentRoi == 1) {
		left = x + 50;
		top = y + 4;
		right = x + ROI_BBOX_WIDTH + 100;
		bottom = y + ROI_BBOX_HEIGHT + 4;
	}

	
}

void CRoi::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	float x_mario, y_mario;
	mario->GetInstance()->GetPosition(x_mario, y_mario);
	if (mario->GetInstance()->nx > 0)
		SetPosition(x_mario - 30, y_mario - 4);
	else SetPosition(x_mario - 90, y_mario - 4);
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
	if (coEvents.size() == 0){}
	else
	{
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			if (coEvents.at(i)->type == GOOMBA_TYPE)
				coEvents.at(i)->dead = true;
			else if (coEvents.at(i)->type == NEN_TYPE) {
				coEvents.at(i)->SetState(CANDLE_STATE_1);
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
	ani = mario->GetInstance()->currentRoi;
	if (animations[ani]->isLastFrame) {
		dead = true;
	}
	
	animations[ani]->Render(x, y);
	RenderBoundingBox();
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