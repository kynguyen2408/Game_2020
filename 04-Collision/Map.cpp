#include "Map.h"
Map* Map::_instance = NULL;
Map::Map()
{

}
void Map::LoadResources()
{
	ifstream File;
	char gridFileName[30];
	sprintf_s(gridFileName, "Level1.txt");
	File.open(gridFileName);
	File >> col >> row;
	mapTiles = new int* [row];
	for (int r = 0; r < row; ++r)
	{
		mapTiles[r] = new int[col];
		for (int c = 0; c < col; ++c)
		{
			File >> mapTiles[r][c];
		}
	}
	File.close();
}
void Map::Render()
{
	CTextures* textures = CTextures::GetInstance();
	LPDIRECT3DTEXTURE9 texMap;
	texMap = textures->Get(99);
	CSprites* sprites = CSprites::GetInstance();
	sprites->Add(1111, 0, 0, 32, 1568, texMap);

	CSprite* sprite = sprites->Get(1111);

	for(int i=0;i<row;++i)
		for (int j = 0; j < col; ++j)
		{
			RECT r;
			r.left = j * 32;
			r.top = i * 32;
			r.right = r.left + 32;
			r.bottom = r.top + 32;
			//if (IsCollision(cam->GetBound, r)) // if title rect touch the Camera rect then do not draw 
				sprite->Draw(r.left, r.top + 80, 32 * mapTiles[i][j] - 32, 0, 32 * mapTiles[i][j], 32);
		}

}


void Map::Update(float dt)
{

}
Map* Map::GetInstance()
{
	if (_instance == NULL)
		_instance = new Map();
	return _instance;
}
Map::~Map() {}