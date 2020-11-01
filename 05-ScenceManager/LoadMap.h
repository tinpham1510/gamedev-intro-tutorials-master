#pragma once
#include"Textures.h"
#include <iostream>
#include <fstream>
#include "Game.h"
#define FrameHeight 16
#define FrameWidth	16
#define MAX	200
class LoadMap
{
	int Collumns;
	int Rows;
	int TileSetWidth;
	int TileMapID[MAX][MAX];
	CTextures* textures = new CTextures();
public:
	LoadMap();
	void ReadMap();
	void DrawMap();
	~LoadMap();
};
