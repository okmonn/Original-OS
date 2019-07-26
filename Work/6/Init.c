/*-----インクルード-----*/
#include "Declaration.h"

// カラーテーブル
const unsigned char colorTbl[] = {
	0x00, 0x00, 0x00,
	0xff, 0x00, 0x00,
	0x00, 0xff, 0x00,
	0xff, 0xff, 0x00,
	0x00, 0x00, 0xff,
	0xff, 0x00, 0xff,
	0x00, 0xff, 0xff,
	0xff, 0xff, 0xff,
	0xc6, 0xc6, 0xc6,
	0x84, 0x00, 0x00,
	0x00, 0x84, 0x00,
	0x84, 0x84, 0x00,
	0x00, 0x00, 0x84,
	0x84, 0x00, 0x84,
	0x00, 0x84, 0x84,
	0x84, 0x84, 0x84,
};

// マウスカーソル
const char cursor[16][16] = {
	"**************..",
	"*OOOOOOOOOOO*...",
	"*OOOOOOOOOO*....",
	"*OOOOOOOOO*.....",
	"*OOOOOOOO*......",
	"*OOOOOOO*.......",
	"*OOOOOOO*.......",
	"*OOOOOOOO*......",
	"*OOOO**OOO*.....",
	"*OOO*..*OOO*....",
	"*OO*....*OOO*...",
	"*O*......*OOO*..",
	"**........*OOO*.",
	"*..........*OOO*",
	"............*OO*",
	".............***"
};

// GDTのセット
void Set_GDT(struct GDT* gdt, unsigned int limit, int base, int ar)
{
	if(limit > 0xfffff)
	{
		ar |= 0x8000;
		limit /= 0x1000;
	}

	gdt->limit_low    = limit & 0xffff;
	gdt->base_low     = base & 0xffff;
	gdt->base_mid     = (base >> 16) & 0xff;
	gdt->access_right = ar & 0xff;
	gdt->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	gdt->base_high    = (base >> 24) & 0xff;
}

// IDTのセット
void Set_IDT(struct IDT* idt, int offset, int selector, int ar)
{
	idt->offset_low   = offset & 0xffff;
	idt->selector     = selector;
	idt->dw_count     = (ar >> 8) & 0xff;
	idt->access_right = ar & 0xff;
	idt->offset_high  = (offset >> 16) & 0xffff;
}

// キーボード割り込み処理
void inthandler21(int* esp)
{
	struct BOOTINFO* info = (struct BOOTINFO*)0x0ff0;
	DrawBox(info, 0, 0, 32 * 8 - 1, 15, COL8_000000);
	DrawString(info, "INT 21 (IRQ-1) : PS/2 keyboard", 0, 0, COL8_FFFFFF);

	while(1)
	{
		io_hlt();
	}
}

// マウス割り込み処理
void inthandler2c(int* esp)
{
	struct BOOTINFO* info = (struct BOOTINFO*)0x0ff0;
	DrawBox(info, 0, 0, 32 * 8 - 1, 15, COL8_000000);
	DrawString(info, "INT 2C (IRQ-12) : PS/2 mouse", 0, 0, COL8_FFFFFF);

	while(1)
	{
		io_hlt();
	}
}

// 初期化
void Init(struct BOOTINFO* info, char* mouse)
{
	//記述子初期化
	{
		struct GDT* gdt = (struct GDT*)0x00270000;
		struct IDT* idt = (struct IDT*)0x0026f800;
	
		int i = 0;
		for(; i <= 8191; ++i)
		{
			Set_GDT(gdt + i, 0, 0, 0);
		}
		Set_GDT(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
		Set_GDT(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
		load_gdt(0x0000ffff, 0x00270000);
	
		i = 0;
		for(; i <= 255; ++i)
		{
			Set_IDT(idt + i, 0, 0, 0);
		}
		load_idt(0x7ff, 0x0026f800);

		//割り込み処理登録
		Set_IDT(idt + 0x21, (int)asm_inthandler21, 2 * 8, 0x008e);
		Set_IDT(idt + 0x2c, (int)asm_inthandler2c, 2 * 8, 0x008e);
	}

	//PICの初期化
	{
		io_out8(PIC0_IMR, 0xff); //すべての割り込み禁止
		io_out8(PIC1_IMR, 0xff); //すべての割り込み禁止

		io_out8(PIC0_ICW1, 0x11);   //エッジトリガモード
		io_out8(PIC0_ICW2, 0x20);   //IRQ0-7はIRQ20-27で受け取る
		io_out8(PIC0_ICW3, 1 << 2); //PIC1はIQR2にて接続
		io_out8(PIC0_ICW4, 0x01);   //ノンバッファモード

		io_out8(PIC1_ICW1, 0x11); //エッジトリガモード
		io_out8(PIC1_ICW2, 0x28); //IRQ8-15は、INT28-2fで受ける
		io_out8(PIC1_ICW3, 2);    //PIC1はIRQ2にて接続
		io_out8(PIC1_ICW4, 0x01); //ノンバッファモード

		io_out8(PIC0_IMR, 0xfb); //11111011 PIC1以外は全て禁止
		io_out8(PIC1_IMR, 0xff); //11111111 全ての割り込みを受け付けない
	}

	//CPU割り込み禁止を解除
	io_sti();

	//色情報セット
	{
		int flag = io_load_eflags();
		io_cli();
		io_out8(0x03c8, 0);

		const int cell = 3;
		int i = 0;
		for(; i < sizeof(colorTbl) / sizeof(colorTbl[0]); ++i)
		{
			int n = 0;
			for(; n < cell; ++n)
			{
				io_out8(0x03c9, colorTbl[cell * i + n] / 4);
			}
		}
		io_store_eflags(flag);
	}

	//ウィンドウを作成
	{
		DrawBox(info, 0, 0,                info->scrnX - 1, info->scrnY - 29, COL8_008484);
		DrawBox(info, 0, info->scrnY - 28, info->scrnX - 1, info->scrnY - 28, COL8_C6C6C6);
		DrawBox(info, 0, info->scrnY - 27, info->scrnX - 1, info->scrnY - 27, COL8_FFFFFF);
		DrawBox(info, 0, info->scrnY - 26, info->scrnX - 1, info->scrnY -  1, COL8_C6C6C6);

		DrawBox(info,  3, info->scrnY - 24, 59, info->scrnY - 24, COL8_FFFFFF);
		DrawBox(info,  2, info->scrnY - 24,  2, info->scrnY -  4, COL8_FFFFFF);
		DrawBox(info,  3, info->scrnY -  4, 59, info->scrnY -  4, COL8_848484);
		DrawBox(info, 59, info->scrnY - 23, 59, info->scrnY -  5, COL8_848484);
		DrawBox(info,  2, info->scrnY -  3, 59, info->scrnY -  3, COL8_000000);
		DrawBox(info, 60, info->scrnY - 24, 60, info->scrnY -  3, COL8_000000);

		DrawBox(info, info->scrnX - 47, info->scrnY - 24, info->scrnX -  4, info->scrnY -  24, COL8_848484);
		DrawBox(info, info->scrnX - 47, info->scrnY - 23, info->scrnX - 47, info->scrnY -   4, COL8_848484);
		DrawBox(info, info->scrnX - 47, info->scrnY -  3, info->scrnX -  4, info->scrnY -   3, COL8_FFFFFF);
		DrawBox(info, info->scrnX -  3, info->scrnY - 24, info->scrnX -  3, info->scrnY -   3, COL8_FFFFFF);
	}

	//マウスカーソル作成
	{
		const int cell = 16;
		int y = 0;
		for(; y < cell; ++y)
		{
			int x = 0;
			for(; x < cell; ++x)
			{
				if(cursor[y][x] == '*')
				{
					mouse[y * cell + x] = COL8_000000;
				}
				else if(cursor[y][x] == 'O')
				{
					mouse[y * cell + x] = COL8_FFFFFF;
				}
				else if(cursor[y][x] == '.')
				{
					mouse[y * cell + x] = COL8_008484;
				}
			}
		}
	}
}
