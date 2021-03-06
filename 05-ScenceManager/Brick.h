#pragma once
#include "GameObject.h"



class CBrick : public CGameObject
{
public:
	int width;
	int height;
	int num;
	CBrick(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};