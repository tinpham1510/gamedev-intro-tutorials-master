#include "QuestionBrick.h"
void QuestionBrick::Render()
{
	if (CheckBrickCollision == false)
	{
		animation_set->at(BRICK_ANI_QUESTION)->Render(x, y);
	}
	else
	{
		animation_set->at(BRICK_ANI_EMPTY)->Render(x, y);
	}
	//RenderBoundingBox();
}

void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (CheckBrickCollision == false)
	{
		if (vy == 0)
		{
			First_y = y;
		}

		if (First_y - y >= MAX_HEIGHT)
		{
			vy = 0.2;
		}

		if (First_y - y < 0)
		{
			SetState(BRICK_STATE_EMPTY);
		}
	}
}
void QuestionBrick::SetState(int state)
{
	if (CheckBrickCollision == false)
	{
		switch (state)
		{
		case BRICK_STATE_COLLISION:
			vy = -0.2;
			break;
		case BRICK_STATE_EMPTY:
			vy = 0;
			if (y >= First_y)
				y = First_y;
			CheckBrickCollision = true;
			break;

		}
	}
}
void QuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + Height;
}
QuestionBrick::QuestionBrick()
{

}