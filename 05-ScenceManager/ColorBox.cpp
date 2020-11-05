#include "ColorBox.h"

void ColorBox::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void ColorBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
ColorBox::ColorBox(float l, float t, float r, float b)
{
	x = l;
	y = t;
	width = r;
	height = b;
}