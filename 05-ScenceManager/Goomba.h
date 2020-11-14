#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_DEFLECT_SPEED_DIE	0.05f
#define GOOMBA_DEFLECT_Y_SPEED_DIE 0.1f
#define TimeDisappear	100
#define TimeATTACKDisappear	500


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_ON_ATTACKING	300

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_DIE_DOWN	2
class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public: 	
	CGoomba();
	int TimetoDisappear = 0;
	int TimeAttackDisappear = 0;
	virtual void SetState(int state);
	virtual void Reset();
	~CGoomba();
};