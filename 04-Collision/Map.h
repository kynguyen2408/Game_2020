#pragma once
#include "Sprites.h"
#include "Textures.h"
#include <fstream>


class Map
{
private:
	static Map* _instance;
	int col;
	int row;
	int** mapTiles;
public:
	Map();
	~Map();
	float cam_x = 0.0f;
	float cam_y = 0.0f;
	void LoadResources(int level);
	void Render(int level);
	void Update(float dt);
	static Map* GetInstance();
	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	//bool IsCollision(RECT rect1, RECT rect2);

};

