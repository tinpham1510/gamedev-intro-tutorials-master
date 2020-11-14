#include "Koopas.h"
#include"Brick.h"
#include"ColorBox.h"
#include "Pipe.h"
#include "FirePlant.h"

#define KOOPAS_HEAD_COLOR_BOX_X	512
#define KOOPAS_LAST_COLOR_BOX_X	586


CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
	isKicked = false;
	Collision = false;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_DIE_SHELL || state== KOOPAS_STATE_SHELL_SLIPERY)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (state == KOOPAS_STATE_SHELL_SLIPERY &&( x < KOOPAS_HEAD_COLOR_BOX_X || x > KOOPAS_LAST_COLOR_BOX_X))
	{
		vy += KOOPAS_GRAVITY * dt;
	}
	// gravity 
	if (state == KOOPAS_STATE_DIE_COLLISION_BULLET)
	{
		vy += KOOPAS_GRAVITY * dt;
	}
	//State Reborn Koopas when koopas die
	if (state == KOOPAS_STATE_DIE )
	{
		if (GetTickCount64() - Reborn >= TimeReborn)
		{
			SetState(KOOPAS_STATE_WALKING);
			SetPosition(x, y - CONST);
			Reborn = 0;
		}
	}
	//
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx < 0 && x < KOOPAS_HEAD_COLOR_BOX_X)
		{
			x = KOOPAS_HEAD_COLOR_BOX_X;
			vx = -vx;
		}

		if (vx > 0 && x > KOOPAS_LAST_COLOR_BOX_X) {
			x = KOOPAS_LAST_COLOR_BOX_X;
			vx = -vx;
		}
	}

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	if (state == KOOPAS_STATE_DIE_SHELL && GetTickCount64() - TimeDeflect >= TIMEDEFLECT)
	{
		vy += KOOPAS_GRAVITY * dt;
		TimeDeflect = 0;
	}
	//set time die
	if (state == KOOPAS_STATE_DIE_COLLISION_BULLET && GetTickCount64() - TimeDie >= TIMEDIE)
	{
		Collision = true;
	}

	if (state == KOOPAS_STATE_SHELL_SLIPERY && GetTickCount64() - TimeDie >= TIMESHELLDIE)
	{
		Collision = true;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<FirePlant*>(e->obj))
			{
				FirePlant* fp = dynamic_cast<FirePlant*>(e->obj);
				if (e->nx != 0 || e->ny < 0)
				{
					fp->Collision = true;
				}
			}
			else if (dynamic_cast<Pipe*>(e->obj) || dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					//	DebugOut(L"9\n");
					vx = -vx;
					//DebugOut(L"vx 2 =%f\n", vx);
				}
			}
			else if (dynamic_cast<CMario*>(e->obj))
			{
				if (e->nx != 0)
				{
					if (state == KOOPAS_STATE_SHELL_SLIPERY || state == KOOPAS_STATE_WALKING)
					{
						x += dx;
					}
				}
			}
			else if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* qb = dynamic_cast<QuestionBrick*>(e->obj);
				if (state == KOOPAS_STATE_SHELL_SLIPERY)
				{
					if (e->nx != 0)
					{
						qb->SetState(BRICK_STATE_COLLISION);
						vx = -vx;
					}
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (state == KOOPAS_STATE_SHELL_SLIPERY)
				{
					if (e->nx != 0 || e->ny < 0)
					{
						if (nx == 1)
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
			else 
			{
				if (e->nx != 0)
				{
					x += dx;
				}
			}

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"vx 1 =%f\n", vx);
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_SHELL_SLIPERY)
	{
		ani = KOOPAS_SHELL_SLIP_RIGHT;
	}
	else if (state == KOOPAS_STATE_DIE_SHELL || state == KOOPAS_STATE_DIE_COLLISION_BULLET)
	{
		ani = KOOPAS_SHELL_DIE;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx < 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);
	

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = new CMario();
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		Reborn = GetTickCount64();
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
		vy = 0;
		break;
	case KOOPAS_STATE_DIE_SHELL:
		vy = -KOOPAS_DEFLECT_SPEED_SHELL;
		vx = 0;
		TimeDeflect = GetTickCount64();
		break;
	case KOOPAS_STATE_SHELL_SLIPERY:
		vx = KOOPAS_SPEED_SHELL * nx;
		mario->IsKicked = true;
		TimeDie = GetTickCount64();
		break;
	case KOOPAS_STATE_DIE_COLLISION_BULLET:
		vy = -KOOPAS_SPEED_DEFLECT_Y;
		vx = KOOPAS_SPEED* nx;
		TimeDie = GetTickCount64();
		break;
	}

}
void CKoopas::Reset()
{
	SetState(KOOPAS_STATE_WALKING);
	Collision = false;
}