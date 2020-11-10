#pragma once 
#include "Scene.h"



class PlayScene : public Scene
{
public:

	PlayScene();
	void LoadResources();
	void Update(float dt);
	void CreateFileGird(int level);

	void Render();
	


private:
	

};
