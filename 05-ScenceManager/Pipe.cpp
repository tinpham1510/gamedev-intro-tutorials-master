#include "Pipe.h"

void Pipe::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
Pipe::Pipe(float l, float t, float r, float b)
{
	x = l;
	y = t;
	width = r;
	height = b;
}