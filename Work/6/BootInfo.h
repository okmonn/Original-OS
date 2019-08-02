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

	// �R���X�g���N�^
	BootInfo() {
		cyls = leds = vmode = reserve = 0;
		sizeX = sizeY = 0;
		vram = nullptr;
	}
	// �f�X�g���N�^
	~BootInfo() {}
};
