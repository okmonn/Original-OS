#pragma once

class BootInfo
{
public:
	char cyls;
	char leds;
	char vmode;
	char reserve;
	short sizeX;
	short sizeY;
	unsigned char* vram;

	// コンストラクタ
	BootInfo() {
		cyls = leds = vmode = reserve = 0;
		sizeX = sizeY = 0;
		vram = nullptr;
	}
	// デストラクタ
	~BootInfo() {}
};
