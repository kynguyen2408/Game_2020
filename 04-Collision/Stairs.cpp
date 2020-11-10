#include "Stairs.h"

void CStairs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}
CStairs::CStairs()
{
	
}
CStairs::CStairs(int t)
{
	type = t;
}
void CStairs::Render()
{
	RenderBoundingBox();
}
void CStairs::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}