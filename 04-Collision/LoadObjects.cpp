#include "LoadObjects.h"
#include "Brick.h"
#include "Goomba.h"
#include "Panther.h"
#include "Riu.h"
#include "Fire.h"
#include "Nen.h"
#include "Aquaman.h"
#include "Bat.h"

#include <string>
#include <fstream>
using namespace std;

void LoadObjects::CreateFileGird(int level)
{
	ifstream File;

	char gridFileName[30];

	sprintf_s(gridFileName, "text\\Grid%d.txt", level);
	File.open(gridFileName);

	float posX, posY, width, height, id, posTarX, PosMoveSimonX, PosMoveSimonY;

	string str, type;
	while (!File.eof())
	{
		File >> str;		// read type of obj
		if (str == "FIRE_PILLAR")
		{
			File >> posX >> posY >> width >> height >> str;
			Nen* nen = new Nen();
			if (str == "MORNING_STAR") {
				nen->setTypeItems(NEN_ANI_ITEM_WHIP);
			}
			else if (str == "BIG_HEART") {
				nen->setTypeItems(NEN_ANI_ITEM_BIG_HEART);
			}
			else if (str == "KNIFE") {
				nen->setTypeItems(NEN_ANI_ITEM_KNIFE);
			}
			else if (str == "DOUBLE_SHOT") {
				nen->setTypeItems(NEN_ANI_ITEM_DOUBLESHOT);
			}
			nen->SetPosition(posX, posY);
			nen->SetWH(width, height);
			//objects.push_back(nen);

		}
		else if (str == "BRICK") {

			File >> posX >> posY >> width >> height;
			CBrick * brick = new CBrick();
			brick->SetPosition(posX, posY);
			brick->SetWH(width, height);

		}
		else if (str == "GHOST") {
			File >> posX >> posY >> width >> height;
			CGoomba * ghost = new CGoomba();
			ghost->SetPosition(posX, posY);
			ghost->SetWH(width, height);
		}
		else if (str == "BLACKPANTHER") {
			File >> posX >> posY >> width >> height;
			CPanther * panther = new CPanther();
			panther->SetPosition(posX, posY);
			panther->SetWH(width, height);
		}
		else if (str == "BAT") {
			File >> posX >> posY >> width >> height;
			CBat * bat = new CBat();
			bat->SetPosition(posX, posY);
			bat->SetWH(width, height);
		}
		else if (str == "AQUAMAN") {
			File >> posX >> posY >> width >> height;
			CAquaman * aquaman = new CAquaman();
			aquaman->SetPosition(posX, posY);
			aquaman->SetWH(width, height);
		}
		//else if (str == "STAIR") {
		//	GridFile >> posX >> posY >> width >> height;
		//	CAquaman * aquaman = new CAquaman();
		//	aquaman->SetPosition(posX, posY);
		//	aquaman->SetWH(width, height);
		//}

		//case STAIR:
		//{
		//	GridFile >> posX >> posY >> width >> height >> type;
		//	GridFile >> left >> top >> right >> bot;
		//	auto stair = new CStair(TYPEString[type]);
		//	stair->SetPosition(posX, posY);
		//	stair->width = width;
		//	stair->height = height;

		//	loop(r, top, bot)
		//		loop(c, left, right)
		//		cells[r][c]->objects.insert(stair);
		//	break;
		//}
		}

	File.close();

}
