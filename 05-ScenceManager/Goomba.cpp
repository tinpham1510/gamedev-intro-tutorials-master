#include "Goomba.h"
#include "Brick.h"
#include"Pipe.h"
#include"FirePlant.h"
#include "Mario.h"
#define GOOMBA_GRAVITY	0.0005f

CGoomba::CGoomba()
{
	Collision = false;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIE_ON_ATTACKING)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	//
	if (state == GOOMBA_STATE_DIE)
		if (GetTickCount64() - TimetoDisappear >= TimeDisappear)
			Collision = true;

	if (state == GOOMBA_STATE_DIE_ON_ATTACKING)
		if (GetTickCount64() - TimeAttackDisappear >= TimeATTACKDisappear)
			Collision = true;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}
	vy += GOOMBA_GRAVITY * dt;
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
			if (state == GOOMBA_STATE_DIE_ON_ATTACKING)
			{
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			else
			{
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
						vx = -vx;
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
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE)
	{
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == GOOMBA_STATE_DIE_ON_ATTACKING)
	{
		ani = GOOMBA_ANI_DIE_DOWN;
	}

	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			TimetoDisappear = GetTickCount64();
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_DIE_ON_ATTACKING:
			vy = -GOOMBA_DEFLECT_Y_SPEED_DIE;
			vx = GOOMBA_DEFLECT_Y_SPEED_DIE * nx;
			TimeAttackDisappear = GetTickCount64();
			break;
	}
}
void CGoomba::Reset()
{
	SetState(GOOMBA_STATE_WALKING);
	Collision = false;
}