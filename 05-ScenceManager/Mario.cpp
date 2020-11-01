#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "PlayScence.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	OnGround = true;
	running = flying = isMaxSpeed = Hitting = false;
	MarioFireBullet* mfl = new MarioFireBullet(0, 0);
	FireBullet.push_back(mfl);
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	if (flying)
	{
		vy += MARIO_GRAVITY_FLYING * dt;
	}
	else
	{
		vy += MARIO_GRAVITY * dt;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// set position in game
	if (x < 0)
	{
		x = 0;
	}
	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if ((!left || !right) && !running)
	{
		if (vx < -MAX_SPEED_WALKING || vx > MAX_SPEED_WALKING)
		{
			vx -=vx ; 
		}
	}
	if (Hitting == true && GetTickCount64() - check >= ATKTIMEEND)
	{
		Hitting = false;
	}
	if (running)
	{
		if (nx < 0)
		{
			if (left == false)
			{
				if (vx < -MAX_SPEED_RUNNING || vx > MAX_SPEED_RUNNING)
				{
					vx -= vx;
				}
			}
		}
		else
		{
			if (right == false)
			{
				if (vx < -MAX_SPEED_RUNNING || vx > MAX_SPEED_RUNNING)
				{
					vx -= vx;
				}
			}
		}
	}
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		if (rdx != 0 && rdx!=dx)
			x += nx*abs(rdx); 
		
		// block every object first!
		x += min_tx*dx + nx*0.5f;
		y += min_ty*dy + ny*0.5f;

		if (ny!=0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick *>(e->obj)|| dynamic_cast<ColorBox *>(e->obj))
			{
				if (e->ny < 0)
				{
					OnGround = true;
					//falling = false;
					flying = false;
					DecreaseVaFlying();
				}
			}
			if (dynamic_cast<ColorBox*>(e->obj))
			{
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	DebugOut(L"vx: %f\n", vx);
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
	if (level == MARIO_LEVEL_BIG_TAIL)
	{
		if (vx == 0)
		{
			if (OnGround == true)
			{
				if (OnSitting == true)//sitting
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_SITTING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_SITTING_LEFT;
					}
					CheckSitting = true;
				}
				else if (Hitting == true) //hitting idle
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_HITTING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_HITTING_LEFT;
					}
				}
				else
				{
					if (nx > 0) ani = MARIO_ANI_BIG_TAIL_IDLE_RIGHT; //d
					else ani = MARIO_ANI_BIG_TAIL_IDLE_LEFT;

				}
			}
			else
			{ 
				if (OnSitting == true)//sitting
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_SITTING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_SITTING_LEFT;
					}
					CheckSitting = true;
				}
				else if ( vy < 0 && flying == false ) //jump idle
				{
						if (nx > 0)
						{
							ani = MARIO_ANI_BIG_TAIL_JUMP_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_BIG_TAIL_JUMP_LEFT;
						}
				}
				else if (vy > 0 && flying==true ) // falling idle while flying
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_FALL_FLYING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_FALL_FLYING_LEFT;
					}

				}
				else if (falling == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_FALLING_S_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_FALLING_S_LEFT;
					}
				}
				else	//falling idle
				 {
					 if (nx > 0)
					 {
						 ani = MARIO_ANI_BIG_TAIL_FALLING_RIGHT;
					 }
					 else
					 {
						 ani = MARIO_ANI_BIG_TAIL_FALLING_LEFT;
					 }
				 }
			}
		}
		else
		{	
			if (OnGround == true)
			{
				if (turnleft == true && flying == false) //turn left
				{
					ani = MARIO_ANI_BIG_TAIL_TURN_LEFT;
				}
				else if (turnright == true && flying == false)//turn right
				{
					ani = MARIO_ANI_BIG_TAIL_TURN_RIGHT;

				}
				else if (isMaxSpeed == true && flying == false) //running
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_TAIL_WALKING_SPEED_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_TAIL_WALING_SPEED_LEFT;

					}
				}
				else if (Hitting == true) //hitting
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_HITTING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_HITTING_LEFT;
					}
				}
				else
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_WALKING_RIGHT;//walk
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_WALKING_LEFT;
					}
				}
			}
			else
			{
				//jumping
				if (flying == false && vy < 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_JUMP_RIGHT;

					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_JUMP_LEFT;
					}
				}
				else if (vy > 0 && flying == true) // falling while flying
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_FALL_FLYING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_FALL_FLYING_LEFT;
					}

				}

				else if (flying == true)
				{

					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_FLYING_RIGHT;

					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_FLYING_LEFT;
					}
				}
				else if (falling == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_FALLING_S_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_FALLING_S_LEFT;
					}
				}
				else  // falling
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_FALLING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_FALLING_LEFT;
					}

				}
			}

		}
	}
	
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (OnGround == false)
			{
				if (vy < 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_SMALL_JUMP_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_JUMP_LEFT;
					}
				}
				else  //falling idle
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_SMALL_FALLING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_FALLING_LEFT;
					}
				}
			}
			else
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
		}
		else
		{
			if (OnGround == false)
			{
				if (vy < 0)
				{
					if (vx > 0)
					{
						ani = MARIO_ANI_SMALL_JUMP_RIGHT;

					}
					else
					{
						ani = MARIO_ANI_SMALL_JUMP_LEFT;
					}
				}
				else //falling idle
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_SMALL_FALLING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_FALLING_LEFT;
					}
				}
			}
			else
			{ 
				if (turnleft == true) //turn left
				{
					ani = MARIO_ANI_SMALL_TURN_LEFT;

				}
				else if (turnright == true)//turn right
				{
					ani = MARIO_ANI_SMALL_TURN_RIGHT;

				}
				else if (isMaxSpeed == true) //running
				{
					if (vx > 0)
					{
						ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_RUNNING_LEFT;

					}
				}
				else
				{
					if (nx > 0)
						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
					else ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
			}
		}
	}

	else if(level==MARIO_LEVEL_BIG)
	{
		if (vx == 0)
		{
			if (OnGround == false)
			{
				if (OnSitting == true)//sitting
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_SITTING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_SITTING_LEFT;
					}
				}
				else if (vy < 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_JUMPING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_JUMPING_LEFT;
					}
				}
				else  //falling idle
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_FALLING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_FALLING_LEFT;
					}
				}
			}
			else
			{
				if (OnSitting == true)//sitting
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_SITTING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_SITTING_LEFT;
					}
				}
				else
				{
					if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
					else ani = MARIO_ANI_BIG_IDLE_LEFT;
				}
			}
		}
		else
		{
			if (OnGround == false)
			{
				if (vy < 0)
				{
					if (vx > 0)
					{
						ani = MARIO_ANI_BIG_JUMPING_RIGHT;

					}
					else
					{
						ani = MARIO_ANI_BIG_JUMPING_LEFT;
					}
				}
				else //falling idle
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_FALLING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_FALLING_LEFT;
					}
				}
			}
			else
			{
				if (turnleft == true) //turn left
				{
					ani = MARIO_ANI_BIG_TURN_LEFT;

				}
				else if (turnright == true)//turn right
				{
					ani = MARIO_ANI_BIG_TURN_RIGHT;

				}
				else if (isMaxSpeed == true) //running
				{
					if (vx > 0)
					{
						ani = MARIO_ANI_BIG_RUNNING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_RUNNING_LEFT;

					}
				}
				else
				{
					if (nx > 0)
						ani = MARIO_ANI_BIG_WALKING_RIGHT;
					else ani = MARIO_ANI_BIG_WALKING_LEFT;
				}
			}
		}
	}


	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		isMaxSpeed = false;
		Hitting = false;
		flying = false;
		checkSitting();
		//press right
		if (running == false)
		{
			if (right == true)
			{
				if (vx < 0)
				{
					turnleft = true;
					vx += dec_const;
					if (vx >= 0)
					{
						vx = MARIO_WALKING_SPEED;
					}
				}
				else if (vx < MAX_SPEED_WALKING)
				{
					turnleft = false;
					vx += acc_const;
					if (vx >= MAX_SPEED_WALKING)
					{
						vx = MAX_SPEED_WALKING;
					}
				}
				
			}
			nx = 1;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		isMaxSpeed = false;
		Hitting = false;
		flying = false;
		checkSitting();
		if (running == false)
		{
			if (left == true)
			{
				if (vx > 0)
				{
					turnright = true;
					vx -= dec_const;
					if (vx <= 0)
					{
						vx = -MARIO_WALKING_SPEED;
					}
				}
				else if (vx > -MAX_SPEED_WALKING)
				{
					turnright = false;
					vx -= acc_const;
					if (vx <= -MAX_SPEED_WALKING)
					{
						vx = -MAX_SPEED_WALKING;
					}
				}
				
			}
			nx = -1;
		}
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
			if (OnGround)
			{
				vy = -MARIO_JUMP_SPEED_Y;
				OnGround = false;
			}
		//
		break;
		//
	case MARIO_STATE_IDLE:
		checkSitting();
		setVarFlying();
		turnleft = false;
		turnright = false;
		flying = false;
		//friction 
		if (!left || !right)
		{
			vx -= (min(abs(vx), fric_const) * (nx));
		}
		// Left and Right
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_SITTING:
		vx = 0;
		//set position y 
		if (level != MARIO_LEVEL_SMALL)
		{
			if (!OnSitting)
			{
				OnSitting = true;
				y = y + CONST;
			}
		}
		//
		break;
	case MARIO_STATE_RUNNING_LEFT:
		falling = false;
		if (running)
		{
			IncreaseVaFlying();
			if (vx > 0)
			{
				turnright = true;
				vx -= dec_const;
				if (vx <= 0)
				{
					vx = -MARIO_RUNNING_SPEED;
					isMaxSpeed = false;
					if (flying == false)
					{
						setVarFlying();
					}
				}
			}
			else if (vx > -MAX_SPEED_RUNNING)
			{
				turnright = false;
				vx -= acc_const;
				if (vx <= -MAX_SPEED_RUNNING)
				{
					vx = -MAX_SPEED_RUNNING;
					isMaxSpeed = true;
				}
			}
			nx = -1;
		}
		
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		falling = false;
		if(running)
		{
			IncreaseVaFlying();
			if (vx < 0)
			{
				turnleft = true;
				vx += dec_const;
				if (vx >= 0)
				{
					vx = MARIO_RUNNING_SPEED;
					isMaxSpeed = false;
					if (flying == false)
					{
						setVarFlying();
					}
				}
			}
			else if (vx < MAX_SPEED_RUNNING)
			{
				turnleft = false;
				vx += acc_const;
				if (vx >= MAX_SPEED_RUNNING)
				{
					vx = MAX_SPEED_RUNNING;
					isMaxSpeed = true;
				}
			}
			nx = 1;
		}
		break;
	//case MARIO_STATE_HITTING:
	//	Hitting = true;
	//	break;

	case MARIO_STATE_FLYING:
		falling = false;
		if (level == MARIO_LEVEL_BIG_TAIL)
		{
			if (Power == POWER_RUNNING)
			{
				vy = -FLYING_SPEED;
			}
		}
		break;
	case MARIO_STATE_FALLING:
		if (level == MARIO_LEVEL_BIG_TAIL)
		{
			if (vy > 0 && flying == false && OnGround == false)
			{
				vy = MARIO_GRAVITY_FALLING_SLOW;
				falling = true;
			}
		}
		break;
	}
}
void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	 if (level == MARIO_LEVEL_BIG_TAIL)
	{
		 if (OnSitting == true)
		 {
			 right = x + MARIO_BIG_SITTING_WIDTH;
			 bottom = y + MARIO_BIG_SITTING_HEIGHT;
		 }
		 else
		 {
			 right = x + MARIO_BIG_BBOX_WIDTH;
			 bottom = y + MARIO_BIG_BBOX_HEIGHT;
		 }
	}
	 else if (level == MARIO_LEVEL_BIG)
	 {
		 if (OnSitting == true)
		 {
			 right = x + MARIO_BIG_SITTING_WIDTH;
			 bottom = y + MARIO_BIG_SITTING_HEIGHT;
		 }
		 else
		 {
			 right = x + MARIO_BIG_BBOX_WIDTH;
			 bottom = y + MARIO_BIG_BBOX_HEIGHT;
		 }
	 }
	else
	{
			 right = x + MARIO_SMALL_BBOX_WIDTH;
			 bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	
	}

	
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG_TAIL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	OnGround = false;
}
void CMario::checkSitting()
{
	if (OnSitting)
	{
		OnSitting = false;
		y = y - CONST;
	}
}
void CMario::IncreaseVaFlying()
{
	if (Power < POWER_RUNNING)
	{
		Power++;
	}
}
void CMario::DecreaseVaFlying()
{
	if (Power > 0 )
	{
		Power--;
	}
}
void CMario::setVarFlying()
{
	Power = 0;
}

