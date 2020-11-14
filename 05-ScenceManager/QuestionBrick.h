#pragma once
#include "GameObject.h"


#define Width 16
#define Height 16
#define BRICK_ANI_QUESTION 0
#define BRICK_ANI_EMPTY	1
#define BRICK_STATE_COLLISION 100
#define BRICK_STATE_EMPTY	200
#define MAX_HEIGHT	5
class QuestionBrick : public CGameObject
{
public:
	bool CheckBrickCollision;
	float First_y;
	QuestionBrick();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};