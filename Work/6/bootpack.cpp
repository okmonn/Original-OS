#include "ColorTbl.h"
#include "Func.h"
#include "BootInfo.h"
#include "SegmentDescriptor.h"
#include "GateDescriptor.h"

// キーボードからの割り込み通知
extern "C" void InitHandle21(int* esp)
{
	BootInfo* info = (BootInfo*)ADR_BOOTINFO;
	os::DrawBox(info, 0, 0, 32 * 8 - 1, 15, COL_000000);
	os::DrawString(info, "INT 21 (IRQ-1) : PS/2 keybord", 0, 0, COL_FFFFFF);
	while (1)
	{
		io_hlt();
	}
}

// マウスからの割り込み通知
extern "C" void InitHandle2c(int* esp)
{

}

/*-----エントリーポイント-----*/
int main(void)
{
	BootInfo* info         = (BootInfo*)ADR_BOOTINFO;
	SegmentDescriptor* gdt = (SegmentDescriptor*)ADR_GDT;
	GateDescriptor* idt    = (GateDescriptor*)ADR_IDT;

	os::InitDescriptor(gdt, idt);
	os::InitPIC();
	io_sti(); //CPU割り込み禁止の解除

	os::InitPalette(0, 15, colorTbl);
	os::InitLayout(info);

	os::DrawCursor(info, (info->sizeX - 16) / 2, (info->sizeY - 28 - 16) / 2, COL_008484);

	char s[40];
	os::Print(s, "(%d %d)", (info->sizeX - 16) / 2, (info->sizeY - 28 - 16) / 2);
	os::DrawString(info, s, 0, 0, COL_FFFFFF);

	io_out8(PIC0_IMR, 0xf9); //PIC1とキーボードを許可(11111001)
	io_out8(PIC1_IMR, 0xef); //マウスを許可(11101111)


	while(1)
	{
		io_hlt();
	}

	return 0;
}
