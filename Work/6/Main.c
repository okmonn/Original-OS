/*-----インクルード-----*/
#include "Declaration.h"

// 整数値を文字列に変換
void Convert(unsigned char* str, int integer, int num)
{
	int index = 0;
	if(integer < 0)
	{
		integer = Abs(integer);
		str[index] = '-';
	}

	int decimal = 0;
	int tmp = 0;
	int i = 0;
	for(; i < num; ++i)
	{
		tmp = (int)(Pow(10, num - (i + 1)));
		decimal = integer % tmp;
		integer = integer / tmp;
		str[index++] = '0' + integer;
		integer = decimal;
	}
}

/*-----エントリーポイント-----*/
void Main(void)
{
	struct BOOTINFO* info = (struct BOOTINFO*)0x0ff0;
	char mouse[256];
	Init(info, mouse);

	DrawString(info, "ABC 123", 8, 8, COL8_FFFFFF);
	DrawString(info, "Haribote OS.", 31, 31, COL8_000000);
	DrawString(info, "Haribote OS.", 30, 30, COL8_FFFFFF);
	
	unsigned char num[9];
	Convert(num, (info->scrnX - 16) / 2, 3);
	DrawString(info, num, 30, 50, COL8_FFFFFF);
	DrawCursor(info, mouse, (info->scrnX - 16) / 2, (info->scrnY - 28 - 16) / 2, 16, 16, 16);

	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);

	while(1)
	{
		io_hlt();
	}
}
