#pragma once
#include"GameObject.h"
class ColorBox:public CGameObject
{
public:
	int width;
	int height;
	ColorBox(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

