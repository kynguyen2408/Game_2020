#pragma once
#include "Scene.h"
class ManagerScene
{

private:
	static ManagerScene *__instance;
public:
	ManagerScene();
	~ManagerScene();
	Scene* PresentScene;	//current scence of Manager Scence hold
	void PlayScene(Scene * scene);
	static ManagerScene* GetInstance();
	void LoadScene(Scene *scene); // tam thoi load 

};
