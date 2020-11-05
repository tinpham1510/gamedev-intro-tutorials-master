#pragma once
#include"GameObject.h"
#include"Utils.h"
#define height_bullet	9
#define width_bullet	8
#define FireBullet_Gravity	0.0005f
#define BULLET_ANI_RIGHT	0
#define BULLET_ANI_LEFT		1
class FirePlant : public CGameObject
{
public:
	bool FireBullet = false;
	FirePlant()
	{
		SetState(BULLET_ANI_RIGHT);
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	~FirePlant()
	{

	}
};

