#pragma once

/*-----カラーコード-----*/
enum Color
{
	COL_000000,
	COL_FF0000,
	COL_00FF00,
	COL_FFFF00,
	COL_0000FF,
	COL_FF00FF,
	COL_00FFFF,
	COL_FFFFFF,
	COL_C6C6C6,
	COL_840000,
	COL_008400,
	COL_848400,
	COL_000084,
	COL_840084,
	COL_008484,
	COL_848484
};

/*-----定数宣言-----*/
#define ADR_BOOTINFO 0x00000ff0

#define ADR_IDT		 0x0026f800
#define LIMIT_IDT	 0x000007ff
#define ADR_GDT		 0x00270000
#define LIMIT_GDT	 0x0000ffff
#define ADR_BOTPAK	 0x00280000
#define LIMIT_BOTPAK 0x0007ffff
#define AR_DATA32_RW 0x4092
#define AR_CODE32_ER 0x409a
#define AR_INTGATE32 0x008e

#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_IMR  0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_IMR  0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1

/*-----アセンブリ関数-----*/
extern "C" void io_hlt(void);
extern "C" void io_cli(void);
extern "C" void io_sti(void);
extern "C" void io_stihlt(void);
extern "C" void io_out8(int port, int data);
extern "C" void io_store_eflags(int eflags);
extern "C" int io_load_eflags(void);
extern "C" void load_gdt(int limit, int addr);
extern "C" void load_idt(int limit, int addr);
extern "C" void inithandle21(void);
extern "C" void inithandle2c(void);
