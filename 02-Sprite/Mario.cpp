#include "Game.h"
#include "Mario.h"

CMario::CMario(float x, float y, float vx):CGameObject(x, y)
{
	this->vx = vx;
};

void CMario::Update(DWORD dt)
{
	x += vx*dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}
}

void CMario::Render()
{
	LPANIMATION ani;
	int flag;
	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	// mario
	if (vx > 0)
	{
		
		ani = CAnimations::GetInstance()->Get(500); flag = 1;
	}
	else
	{
		ani = CAnimations::GetInstance()->Get(501); flag = 1;
	}
	// misc


		if (vx==0)
		{
			ani = CAnimations::GetInstance()->Get(510);
			flag = 2;
		}
		
		if(vx==0)
		{
			ani = CAnimations::GetInstance()->Get(520);
			flag = 3;
		}


	switch (flag)
	{
	case 1: ani->Render(x, y); break;
	case 2: ani->Render(x, y); break;
	case 3: ani->Render(x, y); break;
	default: break;
	}
}