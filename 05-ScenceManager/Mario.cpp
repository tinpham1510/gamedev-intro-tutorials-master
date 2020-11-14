#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"

#include "PlayScence.h"
#define FIREPLANT_ANI_ID 5
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_FIRE;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	TimeToFly = 0;
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
	OnGround = true;
	running = flying = isMaxSpeed = Hitting = false;
	getBullet = 2;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	if (flying == true)
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
	// friction when walk
	//count time attack
	if (Hitting == true && GetTickCount64() - check >= ATKTACK_TIME)
	{
		Hitting = false;
	}
	//friction when run
	if (!running)
	{
		if (vx < -MAX_SPEED_WALKING || vx > MAX_SPEED_WALKING)
		{
			vx -= vx;
		}
	}
	if (running)
	{
		if (vx < -MAX_SPEED_RUNNING || vx > MAX_SPEED_RUNNING)
		{
			vx -= vx;
			isMaxSpeed = false;
		}
	}
	//count time flying
	if (Power == POWER_RUNNING &&flying == true && GetTickCount64() - TimeToFly >=TIMEFLY)
	{
		flying = false;
		setVarFlying();
		running = false;
	}

	//
	if (IsKicked == true && GetTickCount64() - TimeKicked >= TIMEKICK)
	{
		IsKicked = false;
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
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (ny!=0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->ny < 0)
				{
					OnGround = true;
					//falling = false;
					flying = false;
					DecreaseVaFlying();
				}
				//CMario* mario = new CMario();
			if (dynamic_cast<ColorBox*>(e->obj)|| dynamic_cast<FirePlant*>(e->obj))
			{
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			//
			if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e->obj);
				if (e->ny > 0)
				{
					qb->SetState(BRICK_STATE_COLLISION);
				}
			}
			//
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				// Mario Attack tail
				if (level == MARIO_LEVEL_BIG_TAIL)
				{
					if (untouchable == 0)
					{
						if (Hitting == true && e->nx != 0)
						{
							CMario* mario = new CMario();
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								if (mario->nx == 1)
								{
									goomba->nx = -1;
									goomba->SetState(GOOMBA_STATE_DIE_ON_ATTACKING);
								}
								else
									goomba->nx = 1;
									goomba->SetState(GOOMBA_STATE_DIE_ON_ATTACKING);
							}
						}
					}
				}
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
						if (goomba->GetState()!=GOOMBA_STATE_DIE && Hitting==false)
						{
							if (level > 0)
							{
								level--;
								StartUntouchable();
								x += dx;
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba

			if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is koopas
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				 
				if (level == MARIO_LEVEL_BIG_TAIL)
				{
					if (untouchable == 0)
					{
						if (koopas->GetState() != KOOPAS_STATE_DIE)
						{
							if (Hitting == true && e->nx != 0)
							{
								koopas->SetState(KOOPAS_STATE_DIE_SHELL);
								
							}
						}
					}
				}
				// jump on top >> kill Koopas and deflect a bit 

				if (e->ny < 0)
				{
					if (koopas->GetState() != KOOPAS_STATE_DIE)
					{
						koopas->SetState(KOOPAS_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;

					}
				}
				else if (e->nx != 0)
				{
					
					if (untouchable == 0)
					{
						if (koopas->GetState() != KOOPAS_STATE_DIE && Hitting==false)
						{
								if (level > 0)
								{
									level--;
									StartUntouchable();
									x += dx;
								}
								else
									SetState(MARIO_STATE_DIE);
						}
						else
						{
							
							CMario* mario = new CMario();
							if (vx > 0)
							{
								koopas->nx = 1;
								koopas->SetState(KOOPAS_STATE_SHELL_SLIPERY);
								
							}
							else
							{
								koopas->nx = -1;
								koopas->SetState(KOOPAS_STATE_SHELL_SLIPERY);
							}
							SetState(MARIO_STATE_KICK);
						}
					}
				}
			} // Koopas
		
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
				else if (flying == true && vy < 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_TAIL_JUMP_FLYING_RIGHT;

					}
					else
					{
						ani = MARIO_ANI_BIG_TAIL_JUMP_FLYING_LEFT;
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

	else if (level == MARIO_LEVEL_FIRE)
	{
		if (vx == 0)
		{
			if (OnGround == false)
			{
				if (OnSitting == true)//sitting
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_FIRE_BIG_SITTING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_BIG_SITTING_LEFT;
					}
				}
				else if (Attack == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_JUMP_FIRE_BULLET_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_JUMP_FIRE_BULLET_LEFT;
					}
				}
				else if (vy < 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_FIRE_BIG_JUMPING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_BIG_JUMPING_LEFT;
					}
				}
				else  //falling idle
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_FIRE_BIG_FALLING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_BIG_FALLING_LEFT;
					}
				}
			}
			else
			{
				if (OnSitting == true)//sitting
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_FIRE_BIG_SITTING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_BIG_SITTING_LEFT;
					}
				}
				else if (IsKicked == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_KICK_FIRE_BULLET_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_KICK_FIRE_BULLET_LEFT;
					}
				}
				else if (Attack == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_FIRE_BULLET_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_FIRE_BULLET_LEFT;
					}
				}
				else
				{
					if (nx > 0) ani = MARIO_ANI_FIRE_BIG_IDLE_RIGHT;
					else ani = MARIO_ANI_FIRE_BIG_IDLE_LEFT;
				}
			}
		}
		else
		{
			if (OnGround == false)
			{
				if (vy < 0  && Attack==false)
				{
					if (vx > 0)
					{
						ani = MARIO_ANI_FIRE_BIG_JUMPING_RIGHT;

					}
					else
					{
						ani = MARIO_ANI_FIRE_BIG_JUMPING_LEFT;
					}
				}
				else if (Attack == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_JUMP_FIRE_BULLET_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_JUMP_FIRE_BULLET_LEFT;
					}
				}
				else //falling idle
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_FIRE_BIG_FALLING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_BIG_FALLING_LEFT;
					}
				}
			}
			else
			{
				if (turnleft == true) //turn left
				{
					ani = MARIO_ANI_FIRE_BIG_TURN_LEFT;

				}
				else if (turnright == true)//turn right
				{
					ani = MARIO_ANI_FIRE_BIG_TURN_RIGHT;

				}
				else if (IsKicked == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_KICK_FIRE_BULLET_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_KICK_FIRE_BULLET_LEFT;
					}
				}
				else if (Attack == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_FIRE_BULLET_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_FIRE_BULLET_LEFT;
					}
				}
				else if (isMaxSpeed == true) //running
				{
					if (vx > 0)
					{
						ani = MARIO_ANI_FIRE_BIG_RUNNING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_BIG_RUNNING_LEFT;

					}
				}
				else
				{
					if (nx > 0)
						ani = MARIO_ANI_FIRE_BIG_WALKING_RIGHT;
					else ani = MARIO_ANI_FIRE_BIG_WALKING_LEFT;
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
		turnleft = false;
		turnright = false;
		checkSitting();
		//press right
		if (running == false)
		{
			if (right == true)
			{
				if (vx < 0)
				{
					vx += dec_const;
					turnleft = true;
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
		turnright = false;
		turnleft = false;
		checkSitting();
		if (running == false)
		{
			if (left == true)
			{
				if (vx > 0)
				{
					vx -= dec_const;
					turnright = true;
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
		if (flying == false && OnGround == true)
		{
			setVarFlying();
		}
		//friction 
		//left
		if ( turnleft==true || turnright==true)
		{
			vx += (min(abs(vx), fric_const) * (nx));
		}
		else if(turnleft==false || turnright==false)
		{
			vx -= (min(abs(vx), fric_const) * (nx));
		}
		// Right

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
					if (flying == false && OnGround == true)
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
					if (flying == false && OnGround==true)
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

	case MARIO_STATE_FLYING:
		if (level == MARIO_LEVEL_BIG_TAIL)
		{
			if (Power == POWER_RUNNING)
			{
				vy = -FLYING_SPEED;
				vx = MARIO_WALKING_SPEED * nx;
				flying = true;
				TimeToFly = GetTickCount64();
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
	case MARIO_STATE_KICK:
		TimeKicked = GetTickCount64();
		IsKicked = true;
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
			 right = x + MARIO_BIG_TAIL_BBOX_WIDTH;
			 bottom = y + MARIO_BIG_TAIL_BBOX_HEIGHT;
		 }
	}
	 else if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
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
	SetLevel(MARIO_LEVEL_FIRE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	OnGround = false;
	getBullet = 2;
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
		Power --;
	}
}
void CMario::setVarFlying()
{
	Power = 0;
}

