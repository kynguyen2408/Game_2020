#include "Camera.h"

CCamera* CCamera::_instance = NULL;
CCamera* CCamera::GetInstance()
{
	if (_instance == NULL) _instance = new CCamera();
	return _instance;
}
void CCamera::Update()
{
	if (x < 0)
		x = 0;
	else if (x > 1500 - 550)
		x = 950;
}