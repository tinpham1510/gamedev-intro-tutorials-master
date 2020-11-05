#include "FirePlant.h"
#include "MarioFireBullet.h"
void FirePlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_ANI_RIGHT:
		vx += 0.05;
	}

}
void FirePlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 316) {
		x = 316; vx = -vx;
	}
	if (FireBullet)
	{
		vy += FireBullet_Gravity * dt;
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
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->ny < 0)
				{

				}
			}
		}
	}
	else
	{
		x += dx;
		y += dy;
	}
}
void FirePlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width_bullet;
	bottom = top + height_bullet;
}
void FirePlant::Render()
{
	int ani = BULLET_ANI_RIGHT;
	if (vx > 0) ani = BULLET_ANI_RIGHT;
	else if (vx <= 0) ani = BULLET_ANI_LEFT;
	//animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}