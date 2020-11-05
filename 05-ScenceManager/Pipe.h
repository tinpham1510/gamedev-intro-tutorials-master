#pragma once
#include"GameObject.h"
class Pipe :public CGameObject
{
public:
	int width;
	int height;
	Pipe(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
