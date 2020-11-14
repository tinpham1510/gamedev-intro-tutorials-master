#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_DEFLECT_SPEED_SHELL 0.05f

#define KOOPAS_BBOX_WIDTH 18
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_DIE_SHELL 300
#define KOOPAS_STATE_REBORN	400
#define KOOPAS_STATE_SHELL_SLIPERY	500
#define KOOPAS_STATE_DIE_COLLISION_BULLET	600

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_SHELL_DIE 3
#define KOOPAS_SHELL_SLIP_RIGHT	4
#define KOOPAS_SHELL_SLIP_LEFT	5

#define KOOPAS_GRAVITY	0.0005f
#define KOOPAS_SPEED_SHELL	0.3f
#define KOOPAS_SPEED_DEFLECT_Y 0.2f
#define KOOPAS_SPEED	0.05f

#define TimeReborn	10000
#define TimeAttackDie	900
#define TIMEDEFLECT	100
#define TIMEDIE	900
#define TIMESHELLDIE	9000

class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CKoopas();
	bool isKicked = false;
	float start_y;
	int Reborn = 0;
	int TimeDeflect = 0;
	int TimeDie = 0;
	virtual void SetState(int state);
	virtual void Reset();
};