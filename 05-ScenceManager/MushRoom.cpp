#include "MushRoom.h"
void MushRoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + MUSHROOM_WIDTH;
	bottom = top + MUSHROOM_HEIGHT;
}
void MushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += MUSHROOM_GRAVITY * dt;
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


		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

		}
	}
}
void MushRoom::Render()
{
	int ani = MUSHROOM_ANI_APPEAR;
	animation_set->at(ani)->Render(x, y);
}
void MushRoom::SetState(int state)
{
	switch (state)
	{
	case MUSHROOM_APPEAR:
		vy = 0;
		break;
	}
}