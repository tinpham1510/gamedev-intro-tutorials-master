#pragma once
#include"GameObject.h"
#define MUSHROOM_GRAVITY 0.0005f

#define MUSHROOM_ANI_ID	9

#define MUSHROOM_HEIGHT	16
#define MUSHROOM_WIDTH	16

#define MUSHROOM_ANI_APPEAR	0

#define MUSHROOM_APPEAR 100
class MushRoom :public CGameObject
{
public:
	MushRoom()
	{
		Collision = false;

	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};

