#pragma once
#include "GameObject.h"
#include"Game.h"
#include"MarioFireBullet.h"
#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_RUNNING_SPEED		0.05f
//0.1f
#define MARIO_JUMP_SPEED_Y		0.3f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.0005f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_GRAVITY_FALLING_SLOW	0.0001f
#define MARIO_GRAVITY_FLYING	0.0001f	

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_SITTING			500
#define MARIO_STATE_RUNNING_LEFT	600
#define MARIO_STATE_RUNNING_RIGHT	650
#define MARIO_STATE_FLYING			700
#define MARIO_STATE_HITTING			800
#define MARIO_STATE_FALLING			900

//SMALL
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3
#define MARIO_ANI_SMALL_JUMP_RIGHT		25
#define MARIO_ANI_SMALL_JUMP_LEFT		26
#define MARIO_ANI_SMALL_TURN_LEFT		27
#define MARIO_ANI_SMALL_TURN_RIGHT			28
#define MARIO_ANI_SMALL_RUNNING_RIGHT		29
#define MARIO_ANI_SMALL_RUNNING_LEFT		30
#define MARIO_ANI_SMALL_FALLING_RIGHT		31
#define MARIO_ANI_SMALL_FALLING_LEFT		32


#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

// BIG TAIL
#define MARIO_ANI_BIG_TAIL_IDLE_RIGHT		0
#define MARIO_ANI_BIG_TAIL_IDLE_LEFT			1
#define MARIO_ANI_BIG_TAIL_WALKING_RIGHT			4
#define MARIO_ANI_BIG_TAIL_WALKING_LEFT			5
#define MARIO_ANI_BIG_TAIL_SITTING_RIGHT			11
#define MARIO_ANI_BIG_TAIL_SITTING_LEFT			12

#define MARIO_ANI_BIG_TAIL_JUMP_RIGHT		9
#define MARIO_ANI_BIG_TAIL_JUMP_LEFT			10

#define MARIO_ANI_BIG_TAIL_TURN_RIGHT		13
#define MARIO_ANI_BIG_TAIL_TURN_LEFT			14
#define MARIO_ANI_TAIL_WALKING_SPEED_RIGHT	15
#define MARIO_ANI_TAIL_WALING_SPEED_LEFT		16
#define MARIO_ANI_BIG_TAIL_FLYING_RIGHT		17
#define MARIO_ANI_BIG_TAIL_FLYING_LEFT		18
#define MARIO_ANI_BIG_TAIL_HITTING_RIGHT		19
#define MARIO_ANI_BIG_TAIL_HITTING_LEFT		20
#define MARIO_ANI_BIG_TAIL_FALLING_RIGHT		21
#define MARIO_ANI_BIG_TAIL_FALLING_LEFT		22
#define MARIO_ANI_BIG_TAIL_FALLING_S_RIGHT	23
#define MARIO_ANI_BIG_TAIL_FALLING_S_LEFT	24
#define MARIO_ANI_BIG_TAIL_JUMP_FLYING_RIGHT	33
#define MARIO_ANI_BIG_TAIL_JUMP_FLYING_LEFT	34
#define MARIO_ANI_BIG_TAIL_FALL_FLYING_RIGHT	35
#define MARIO_ANI_BIG_TAIL_FALL_FLYING_LEFT	36
// BIG MARIO
#define MARIO_ANI_BIG_IDLE_RIGHT	37
#define MARIO_ANI_BIG_IDLE_LEFT		38
#define MARIO_ANI_BIG_WALKING_RIGHT	39
#define MARIO_ANI_BIG_WALKING_LEFT	40
#define MARIO_ANI_BIG_JUMPING_RIGHT	41
#define MARIO_ANI_BIG_JUMPING_LEFT	42
#define MARIO_ANI_BIG_TURN_RIGHT	43
#define MARIO_ANI_BIG_TURN_LEFT		44
#define MARIO_ANI_BIG_RUNNING_RIGHT	45
#define MARIO_ANI_BIG_RUNNING_LEFT	46
#define MARIO_ANI_BIG_FALLING_RIGHT	47
#define MARIO_ANI_BIG_FALLING_LEFT	48
#define MARIO_ANI_BIG_SITTING_RIGHT	49
#define MARIO_ANI_BIG_SITTING_LEFT	50


#define MARIO_ANI_DIE				8

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG 3
#define MARIO_LEVEL_BIG_TAIL 2

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_BIG_SITTING_WIDTH		14
#define MARIO_BIG_SITTING_HEIGHT	18

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000
#define CONST	9
#define CONST_SPEED 0.45f

#define acc_const 0.004f
#define dec_const 0.012f
#define fric_const 0.004f
#define MAX_SPEED_WALKING	0.15f
#define MAX_SPEED_RUNNING	0.3f
#define FLYING_SPEED	0.1f
#define POWER_RUNNING 55

#define CONST_Y 122

#define ATKTACK_TIME	300

class CMario : public CGameObject
{
public:
	int level;
	int untouchable;
	bool OnGround = false;
	bool OnSitting = false;
	bool CheckSitting = false;
	bool turnleft = false;
	bool turnright = false;
	bool left = false;
	bool right = false;
	bool running = false;
	bool isMaxSpeed = false;
	bool onwalking = false;
	bool Hitting = false;
	bool flying = false;
	bool falling = false;
	bool firebullet = false;
	int Power = 0;
	int check = 0;
	int TimeToFly = 0;
	int getBullet = 1;
	vector<MarioFireBullet*> FireBullet;
	CGame *game =  CGame::GetInstance();
	CKeyEventHandler* Key;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();
	void checkSitting();



	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void IncreaseVaFlying();
	void DecreaseVaFlying();
	void setVarFlying();
};