#include "Koopas.h"
#include"Brick.h"
#include"ColorBox.h"
CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	//left = x;
	//top = y;
	//right = x + KOOPAS_BBOX_WIDTH;

	//if (state == KOOPAS_STATE_DIE)
	//	bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	//else
	//	bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 352) {
		x = 352; vx = -vx;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
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
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<ColorBox*>(e->obj))
			{
				if (e->nx != 0)
				{
					vx = 0;
				}
			}
		}
	}
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	//animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
	}

}