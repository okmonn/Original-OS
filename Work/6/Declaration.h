/*-----定数宣言-----*/
#define COL8_000000  0
#define COL8_FF0000  1
#define COL8_00FF00  2
#define COL8_FFFF00  3
#define COL8_0000FF  4
#define COL8_FF00FF  5
#define COL8_00FFFF  6
#define COL8_FFFFFF  7
#define COL8_C6C6C6  8
#define COL8_840000  9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

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
void io_sti(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdt(int limit, int addr);
void load_idt(int limit, int addr);
void asm_inthandler21(void);
void asm_inthandler2c(void);

/*-----プロトタイプ宣言-----*/
double Abs(int integer);
double Pow(unsigned int integer, int num);
void Convert(unsigned char* str, int integer, int num);
void DrawString(struct BOOTINFO* info, unsigned char* str, int posX, int posY, char color);
void DrawBox(struct BOOTINFO* info, int x1, int y1, int x2, int y2, unsigned char color);
void DrawCursor(struct BOOTINFO* info, char* mouse, int posX, int posY, int sizeX, int sizeY, int cell);
void Set_GDT(struct GDT* gdt, unsigned int limit, int base, int ar);
void Set_IDT(struct IDT* idt, int offset, int selector, int ar);
void inthandler21(int* esp);
void inthandler2c(int* esp);
void Init(struct BOOTINFO* info, char* mouse);
