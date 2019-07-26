/*-----インクルード-----*/

/*-----定数宣言-----*/
#define COL8_000000     0
#define COL8_FF0000     1
#define COL8_00FF00     2
#define COL8_FFFF00     3
#define COL8_0000FF     4
#define COL8_FF00FF     5
#define COL8_00FFFF     6
#define COL8_FFFFFF     7
#define COL8_C6C6C6     8
#define COL8_840000     9
#define COL8_008400     10
#define COL8_848400     11
#define COL8_000084     12
#define COL8_840084     13
#define COL8_008484     14
#define COL8_848484     15

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

// BOOT_INFO構造体
struct BOOTINFO
{
	char cyls;
	char leds;
	char vmode;
	char reserve;
	short scrnX;
	short scrnY;
	char* vram;
};

// 大域セグメント記述子
struct GDT
{
	short limit_low;
	short base_low;
	char base_mid;
	char access_right;
	char limit_high;
	char base_high;
};

// 割り込み記述子
struct IDT
{
	short offset_low;
	short selector;
	char dw_count;
	char access_right;
	short offset_high;
};

/*-----アセンブリ関数宣言-----*/
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdt(int limit, int addr);
void load_idt(int limit, int addr);

/*-----プロトタイプ宣言-----*/
double Abs(int integer);
double Pow(unsigned int integer, int num);
void Convert(unsigned char* str, int integer, int num);
void DrawString(struct BOOTINFO* info, unsigned char* str, int posX, int posY, char color);
void DrawBox(struct BOOTINFO* info, int x1, int y1, int x2, int y2, unsigned char color);
void DrawCursor(struct BOOTINFO* info, char* mouse, int posX, int posY, int sizeX, int sizeY, int cell);
void Set_GDT(struct GDT* gdt, unsigned int limit, int base, int ar);
void Set_IDT(struct IDT* idt, int offset, int selector, int ar);
void Init(struct BOOTINFO* info, char* mouse);

/*-----関数の実装-----*/
// 絶対値
double Abs(int integer)
{
	double tmp = integer;
	if(tmp < 0)
	{
		tmp *= -1;
	}
	return tmp;
}

// iのnum乗
double Pow(unsigned int integer, int num)
{
	double tmp = 1;
	while(num != 0)
	{
		if(num > 0)
		{
			tmp *= integer;
			--num;
		}
		else
		{
			tmp /= integer;
			++num;
		}
	}
	return tmp;
}

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
	gdt->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0x0f);
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

// 初期化
void Init(struct BOOTINFO* info, char* mouse)
{
	//記述子初期化
	{
		struct GDT* gdt = (struct GDT*)0x00270000;
		struct IDT* idt = (struct IDT*)0x0026f800;
	
		int i = 0;
		for(; i < 8192; ++i)
		{
			Set_GDT(gdt + i, 0, 0, 0);
		}
		Set_GDT(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
		Set_GDT(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
		load_gdt(0xffff, 0x00270000);
	
		i = 0;
		for(; i < 256; ++i)
		{
			Set_IDT(idt + i, 0, 0, 0);
		}
		load_idt(0x7ff, 0x0026f800);
	}
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
	while(1)
	{
		io_hlt();
	}
}
