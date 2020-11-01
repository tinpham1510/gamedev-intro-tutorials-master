#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_GRAVITY			0.1f
#define MARIO_JUMP_HIGH_SPEED	1.0f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DOWN			400
#define MARIO_STATE_HIGH_JUMP		500

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3
#define MARIO_ANI_DOWN_LEFT			4
#define MARIO_ANI_DOWN_RIGHT		5


class CMario : public CGameObject
{
public: 

	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};