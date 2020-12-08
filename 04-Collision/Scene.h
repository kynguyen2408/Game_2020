#pragma once
#include "GlobalConfig.h"
class Scene
{
public:

	virtual void LoadResources(int level) {};
	virtual void Update(float dt) {};
	virtual void Render() {};
	virtual void OnKeyDown(int keyCode) {};
	virtual void OnKeyUp(int keyCode) {};
};
