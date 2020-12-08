#include "ManagerScene.h"

ManagerScene* ManagerScene:: __instance = NULL;

ManagerScene::ManagerScene()
{
	PresentScene = NULL;
}
void ManagerScene::PlayScene(Scene * scene)
{
	delete PresentScene;
	PresentScene = scene;
}
void ManagerScene::LoadScene(Scene * scene)
{
	PresentScene = scene;
	
}
ManagerScene::~ManagerScene()
{
	delete PresentScene;
}
ManagerScene * ManagerScene::GetInstance()
{
	if (__instance == NULL)
	{
		__instance = new ManagerScene();
	}
	return __instance;
}