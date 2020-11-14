#pragma once
#include"GameObject.h"
#include"Utils.h"
#include "Brick.h"
#include "ColorBox.h"
#include "Pipe.h"
#include "Goomba.h"
#include"Mario.h"
#include "Koopas.h"
#include"QuestionBrick.h"

#define height_bullet	9
#define width_bullet	8
#define FireBullet_Gravity	0.0005f
#define SPEED_BULLET	0.2
#define BULLET_DEFLECT	0.1
#define TIMEDISAPPEAR	100

#define BULLET_ANI_RIGHT	0
#define BULLET_ANI_LEFT		1
#define BULLET_ANI_BOOM		2

#define BULLET_STATE_BOOM	1
#define FIREPLANT_ANI_ID 5
class FirePlant : public CGameObject
{
public:
	bool FireBullet = false;
	int TimeCollision = 0;
	int Time = 0;
	bool checkCollision;
	FirePlant(float VX, float VY)
	{
		vx = VX;
		vy = VY;
		Collision = false;
		checkCollision = false;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	~FirePlant()
	{

	}
};

