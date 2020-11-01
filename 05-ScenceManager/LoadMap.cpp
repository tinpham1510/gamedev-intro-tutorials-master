#include"LoadMap.h"
#include"GameObject.h"
LoadMap::LoadMap()
{
	CTextures::GetInstance()->Add(500, L"textures\\Map1-1.png", D3DCOLOR_XRGB(255, 255, 255));
	ReadMap();
}
void LoadMap::ReadMap()
{
	ifstream ifs("textures\\TileMap1.txt", ios::in);
	ifs >> Rows >> Collumns >> TileSetWidth; // rows: 27, columns: 176, tileSetWidth: 11
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Collumns; j++)
		{
			ifs >> TileMapID[i][j];
		}
	}
	ifs.close();
}
void LoadMap::DrawMap()
{
	for (int i = 0; i < Rows; i++)
		for (int j = 0; j < Collumns; j++)
		{
			RECT r;
			int x = (TileMapID[i][j] - 1) % TileSetWidth;// nums column in Map1-1
			int y; // nums row in Map 1-1
			if (TileMapID[i][j] % TileSetWidth == 0)
			{
				y = (TileMapID[i][j] - 1) / TileSetWidth;
			}
			else
			{
				y = TileMapID[i][j] / TileSetWidth;
			}
			r.left = x * FrameWidth;
			r.top =  y * FrameHeight;
			r.right = r.left + FrameWidth;
			r.bottom = r.top + FrameHeight;
			CGame::GetInstance()->Draw(j * FrameWidth, i * FrameHeight, CTextures::GetInstance()->Get(500), r.left, r.top, r.right, r.bottom);
		}
}