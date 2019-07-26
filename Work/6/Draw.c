/*-----インクルード-----*/
#include "Declaration.h"

// ボックス描画
void DrawBox(struct BOOTINFO* info, int x1, int y1, int x2, int y2, unsigned char color)
{
	int y = y1;
	for(; y <= y2; ++y)
	{
		int x = x1;
		for(; x <= x2; ++x)
		{
			info->vram[y * info->scrnX + x] = color;
		}
	}
}

// 文字列表示
void DrawString(struct BOOTINFO* info, unsigned char* str, int posX, int posY, char color)
{
	extern char hankaku[4096];
	char* p, d;
	for(; *str != 0x00; ++str)
	{
		int i = 0;
		for(; i < 16; ++i)
		{
			p = info->vram + (posY + i) * info->scrnX + posX;
			d = (hankaku + *str * 16)[i];
			if ((d & 0x80) != 0) { p[0] = color; }
			if ((d & 0x40) != 0) { p[1] = color; }
			if ((d & 0x20) != 0) { p[2] = color; }
			if ((d & 0x10) != 0) { p[3] = color; }
			if ((d & 0x08) != 0) { p[4] = color; }
			if ((d & 0x04) != 0) { p[5] = color; }
			if ((d & 0x02) != 0) { p[6] = color; }
			if ((d & 0x01) != 0) { p[7] = color; }
		}
		posX += i / 2;
	}
}

// マウスカーソル描画
void DrawCursor(struct BOOTINFO* info, char* mouse, int posX, int posY, int sizeX, int sizeY, int cell)
{
	int y = 0;
	for(; y < sizeY; ++y)
	{
		int x = 0;
		for(; x < sizeX; ++x)
		{
			info->vram[(posY + y) * info->scrnX + (posX + x)] = mouse[y * cell + x];
		}
	}
}
