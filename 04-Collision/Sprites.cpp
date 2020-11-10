#include "Sprites.h"
#include "Game.h"
#include "debug.h"
#include "Textures.h"
#include <fstream>
#include <fstream>
#include <string>
CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

void CSprites::LoadResources()
{
	ifstream File;
	File.open(L"text\\sprites.txt");
	int idSpirtes, left, top, right, bottom, Idtex;
	CTextures * textures = CTextures::GetInstance();
	while (!File.eof())
	{

		File >> idSpirtes >> left >> top >> right >> bottom >> Idtex;
		LPDIRECT3DTEXTURE9 textWhip = textures->Get(Idtex);
		Add(idSpirtes, left, top, right, bottom, textWhip);

	}
	File.close();

}

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}
void CSprite::Draw(float x, float y, float left, float top, float right, float bottom)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, 245);
}



void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}


LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}


void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t=this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame <= -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size() - 1)
			{
				isLastFrame = true;
			}
			if (currentFrame == frames.size())
			{
				currentFrame = 0;
				
			}

		}
		else
		{
			isLastFrame = false;
			t += now - lastFrameTime;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	if (animations[id] != NULL)
	{
		push(id, ani);
	}
	else
		animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	return animations[id];
}

void CAnimations::LoadResources()
{
	ifstream File;
	File.open(L"text\\animations.txt");
	vector<int> ParaAni;
	ParaAni.clear();
	vector<int>::iterator it;
	int reader;
	int time;
	while (!File.eof())
	{
		File >> reader;
		if (reader > -1)
		{
			ParaAni.push_back(reader);
		}
		else
		{
			LPANIMATION ani;
			if (reader < -1)
				ani = new CAnimation(abs(reader));
			else
				ani = new CAnimation(100);
			for (auto it = ParaAni.begin(); it != ParaAni.end() - 1; ++it)
				ani->Add(*it);
			it = ParaAni.end() - 1;
			Add(*it, ani);
			ParaAni.clear();
		}
	}
	File.close();
}
void CAnimations::push(int id, LPANIMATION ani)
{
	for (auto it = ani->frames.begin(); it != ani->frames.end(); ++it)
		animations[id]->frames.push_back(*it);
}