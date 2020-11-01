#include "Brick.h"

void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
CBrick::CBrick(float l, float t, float r, float b)
{
	x = l;
	y = t;
	width = r;
	height = b;
}