#include "FirePlant.h"

void FirePlant::SetState(int state)
{
	switch (state)
	{
	case BULLET_STATE_BOOM:
		checkCollision = true;
		TimeCollision = GetTickCount64();
		break;
	}
}
void FirePlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
		if (FireBullet)
		{
			vy += FireBullet_Gravity * dt;
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
				y += min_ty * dy + ny * 0.4f;
				if (ny != 0) vy = 0;
				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					
					LPCOLLISIONEVENT e = coEventsResult[i];
					if (dynamic_cast<CMario*>(e->obj))
					{
						if (e->nx != 0)
						{
							x += dx;
						}
					}

					if (e->ny < 0)
					{
						vy = -BULLET_DEFLECT; // Bullet deflect
					}
					//

					//
					if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj))
					{
						if (e->nx != 0)
						{
							vx = 0;
							vy = 0;
							SetState(BULLET_STATE_BOOM);
						}
					}
					else if (dynamic_cast<Pipe*>(e->obj))
					{
						if (e->nx != 0 || e->ny < 0)
						{
							
							vx = 0;
							vy = 0;
							SetState(BULLET_STATE_BOOM);
						}
					}
					else if (dynamic_cast<CGoomba*>(e->obj))
					{
						CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
						if (e->nx != 0 || e->ny < 0)
						{
							if (nx == 1)
							{
								goomba->nx = -1;
								goomba->SetState(GOOMBA_STATE_DIE_ON_ATTACKING);
							}
							else
							{
								goomba->nx = 1;
								goomba->SetState(GOOMBA_STATE_DIE_ON_ATTACKING);
								
							}
							vx = 0;
							vy = 0;
							Collision = true;
						}
					}
					else if (dynamic_cast<CKoopas*>(e->obj))
					{
						CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
						if (e->nx != 0 || e->ny < 0)
						{
							if (nx == 1)
							{
								koopas->nx = -1;
								koopas->SetState(KOOPAS_STATE_DIE_COLLISION_BULLET);
							}
							else
							{
								koopas->nx = 1;
								koopas->SetState(KOOPAS_STATE_DIE_COLLISION_BULLET);

							}
							vx = 0;
							vy = 0;
							Collision = true;
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
		else
		{
			x += dx;
			y += dy;
			vx = 0.1;
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
	if (nx == 1)
	{
		ani = BULLET_ANI_RIGHT;
		if (checkCollision == true)
		{
			ani = BULLET_ANI_BOOM;
			if (GetTickCount64() - TimeCollision >= TIMEDISAPPEAR)
			{
				Collision = true;
			}
		}
	}
	else if (nx == -1)
	{
		ani = BULLET_ANI_LEFT;
		if (checkCollision == true)
		{
			ani = BULLET_ANI_BOOM;
			if ( GetTickCount64() - TimeCollision >= TIMEDISAPPEAR)
			{
				Collision = true;
			}
		}
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}