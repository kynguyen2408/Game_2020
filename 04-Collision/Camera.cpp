#include "Camera.h"

CCamera* CCamera::_instance = NULL;
CCamera* CCamera::GetInstance()
{
	if (_instance == NULL) _instance = new CCamera();
	return _instance;
}
void CCamera::Update()
{
	switch (map)
	{
	case 1:
		if (this->x < BEGIN_MAP1)
		{
			this->x = BEGIN_MAP1;

		}
		else if (this->x > END_MAP1 - SCREEN_WIDTH)
		{
			this->x = END_MAP1 - SCREEN_WIDTH;
		}
		break;
	case 2:
	{
		if (this->x < BEGIN_MAP2)
		{
			this->x = BEGIN_MAP2;

		}
		else if (this->x > END_MAP2 - SCREEN_WIDTH)
		{
			this->x = END_MAP2 - SCREEN_WIDTH;
		}
		break;
	}
	case 3:
	{
		if (this->x < BEGIN_MAP3)
		{
			this->x = BEGIN_MAP3;

		}
		else if (this->x > END_MAP3 - SCREEN_WIDTH)
		{
			this->x = END_MAP3 - SCREEN_WIDTH;
		}
		break;
	}
	case 4:
	{
		if (this->x < BEGIN_MAP4)
		{
			this->x = BEGIN_MAP4;
		}
		else if (this->x > END_MAP4 - SCREEN_WIDTH)
		{
			this->x = END_MAP4 - SCREEN_WIDTH;
		}
		this->y = MAP_4_Y;
		break;
	}
	case 5:
	{
		if (this->x < BEGIN_MAP5)
		{
			this->x = BEGIN_MAP5;
		}
		else if (this->x > END_MAP5 - SCREEN_WIDTH)
		{
			this->x = END_MAP5 - SCREEN_WIDTH;
		}
		break;
	}
	case 6:
	{
		this->x = END_MAP5 - SCREEN_WIDTH;
		break;
	}
	default:
		break;
	}
}