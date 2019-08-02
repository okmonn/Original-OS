#include "Func.h"
#include "Font.h"
#include "Cursor.h"
#include "BootInfo.h"
#include "SegmentDescriptor.h"
#include "GateDescriptor.h"
#include <stdarg.h>

// 10�i������ASCII�R�[�h�ɕϊ�
int dec2asc(char* str, int dec)
{
	int len = 0, len_buf; //����
	int buf[10];
	while (1) 
	{ //10�Ŋ��ꂽ�񐔁i�܂茅���j��len�ɁA�e����buf�Ɋi�[
		buf[len++] = dec % 10;
		if (dec < 10) break;
		dec /= 10;
	}
	len_buf = len;
	while (len)
	{
		*(str++) = buf[--len] + 0x30;
	}
	return len_buf;
}

// 16�i������ASCII�R�[�h�ɕϊ�
int hex2asc(char* str, int dec)
{
	int len = 0, len_buf; //����
	int buf[10];
	while (1) 
	{ //10�Ŋ��ꂽ�񐔁i�܂茅���j��len�ɁA�e����buf�Ɋi�[
		buf[len++] = dec % 16;
		if (dec < 16) break;
		dec /= 16;
	}
	len_buf = len;
	while (len) 
	{
		len--;
		*(str++) = (buf[len] < 10) ? (buf[len] + 0x30) : (buf[len] - 9 + 0x60);
	}
	return len_buf;
}

// sprintf�̃I���W�i��
void os::Print(char* str, const char* fmt, ...)
{
	va_list list;
	va_start(list, 2);

	int len = 0;
	while (*fmt) 
	{
		if (*fmt == '%') 
		{
			fmt++;
			switch (*fmt)
			{
			case 'd':
				len = dec2asc(str, va_arg(list, int));
				break;
			case 'x':
				len = hex2asc(str, va_arg(list, int));
				break;
			}
			str += len; fmt++;
		}
		else
		{
			*(str++) = *(fmt++);
		}
	}
	*str = 0x00; //�Ō��NULL��ǉ�
	va_end(list);
}

// �J�[�\���`��
void os::DrawCursor(BootInfo* info, const int& x, const int& y, const Color& color)
{
	for (int i = 0; i < 16; ++i)
	{
		for (int n = 0; n < 16; ++n)
		{
			switch (cursor[i][n])
			{
			case '*':
				info->vram[(y + i) * info->sizeX + (x + n)] = COL_000000;
				break;
			case 'O':
				info->vram[(y + i) * info->sizeX + (x + n)] = COL_FFFFFF;
				break;
			case '.':
				info->vram[(y + i) * info->sizeX + (x + n)] = color;
			default:
				break;
			}
		}
	}
}

// �{�b�N�X�`��
void os::DrawBox(BootInfo* info, const int& x1, const int& y1, const int& x2, const int& y2, const Color& color)
{
	for (int y = y1; y <= y2; ++y)
	{
		for (int x = x1; x <= x2; ++x)
		{
			info->vram[y * info->sizeX + x] = color;
		}
	}
}

// �t�H���g�`��
void os::DrawFont(BootInfo* info, const unsigned char* font, const int& x, const int& y, const Color& color)
{
	for (unsigned int i = 0; i < 16; ++i)
	{
		unsigned char* ptr = info->vram + (y + i) * info->sizeX + x;
		unsigned char data = font[i];
		if ((data & 0x80) != 0) ptr[0] = color;
		if ((data & 0x40) != 0) ptr[1] = color;
		if ((data & 0x20) != 0) ptr[2] = color;
		if ((data & 0x10) != 0) ptr[3] = color;
		if ((data & 0x08) != 0) ptr[4] = color;
		if ((data & 0x04) != 0) ptr[5] = color;
		if ((data & 0x02) != 0) ptr[6] = color;
		if ((data & 0x01) != 0) ptr[7] = color;
	}
}

// ������`��
void os::DrawString(BootInfo* info, const char* str, const int& x, const int& y, const Color& color)
{
	int posX = x;
	for (; *str != 0x00; ++str)
	{
		DrawFont(info, font + *str * 16, posX, y, color);
		posX += 8;
	}
}

// �L�q�q�̏�����
void os::InitDescriptor(SegmentDescriptor* gdt, GateDescriptor* idt)
{
	for (int i = 0; i <= LIMIT_GDT / 8; ++i)
	{
		(gdt + i)->SetParam(0, 0, 0);
	}
	(gdt + 1)->SetParam(0xffffffff, 0x00000000, AR_DATA32_RW);
	(gdt + 2)->SetParam(LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
	load_gdt(LIMIT_GDT, ADR_GDT);

	for (int i = 0; i <= LIMIT_IDT / 8; ++i)
	{
		(idt + i)->SetParam(0, 0, 0);
	}
	load_idt(LIMIT_IDT, ADR_IDT);

	(idt + 0x21)->SetParam((int)inithandle21, 2 * 8, AR_INTGATE32);
	(idt + 0x2c)->SetParam((int)inithandle2c, 2 * 8, AR_INTGATE32);
}

// PIC�̏�����
void os::InitPIC(void)
{
	io_out8(PIC0_IMR, 0xff); //�S�Ă̊��荞�݂��󂯕t���Ȃ�
	io_out8(PIC1_IMR, 0xff); //�S�Ă̊��荞�݂��󂯕t���Ȃ�

	io_out8(PIC0_ICW1, 0x11);   //�G�b�W�g���K���[�h
	io_out8(PIC0_ICW2, 0x20);   //IRQ0-7�́AINT20-27�Ŏ󂯂�
	io_out8(PIC0_ICW3, 1 << 2); //PIC1��IRQ2�ɂĐڑ�
	io_out8(PIC0_ICW4, 0x01);   //�m���o�b�t�@���[�h

	io_out8(PIC1_ICW1, 0x11); //�G�b�W�g���K���[�h
	io_out8(PIC1_ICW2, 0x28); //IRQ8-15�́AINT28-2f�Ŏ󂯂�
	io_out8(PIC1_ICW3, 2);    //PIC1��IRQ2�ɂĐڑ�
	io_out8(PIC1_ICW4, 0x01); //�m���o�b�t�@���[�h

	io_out8(PIC0_IMR, 0xfb); //11111011 PIC1�ȊO�͑S�ċ֎~
	io_out8(PIC1_IMR, 0xff); //11111111 �S�Ă̊��荞�݂��󂯕t���Ȃ�
}

// �J���[�p���b�g�̏�����
void os::InitPalette(const unsigned int& start, const unsigned int& end, const unsigned char* rgb)
{
	const unsigned int block = 3;
	int eflags = io_load_eflags();
	io_cli();
	io_out8(0x03c8, start);
	for (unsigned int i = start; i <= end; ++i)
	{
		for (unsigned int n = 0; n < block; ++n)
		{
			io_out8(0x03c9, rgb[i * block + n] / 4);
		}
	}
	io_store_eflags(eflags);
}

// �E�B���h�E���C�A�E�g�̏�����
void os::InitLayout(BootInfo* info)
{
	DrawBox(info, 0,                0, info->sizeX - 1, info->sizeY - 29, COL_008484);
	DrawBox(info, 0, info->sizeX - 28, info->sizeX - 1, info->sizeY - 28, COL_C6C6C6);
	DrawBox(info, 0, info->sizeY - 27, info->sizeX - 1, info->sizeY - 27, COL_FFFFFF);
	DrawBox(info, 0, info->sizeY - 26, info->sizeX - 1, info->sizeY -  1, COL_C6C6C6);

	DrawBox(info,  3, info->sizeY - 24, 59, info->sizeY - 24, COL_FFFFFF);
	DrawBox(info,  2, info->sizeY - 24,  2, info->sizeY -  4, COL_FFFFFF);
	DrawBox(info,  3, info->sizeY -  4, 59, info->sizeY -  4, COL_848484);
	DrawBox(info, 59, info->sizeY - 23, 59, info->sizeY -  5, COL_848484);
	DrawBox(info,  2, info->sizeY -  3, 59, info->sizeY -  3, COL_000000);
	DrawBox(info, 60, info->sizeY - 24, 60, info->sizeY -  3, COL_000000);

	DrawBox(info, info->sizeX - 47, info->sizeY - 24, info->sizeX -  4, info->sizeY - 24, COL_848484);
	DrawBox(info, info->sizeX - 47, info->sizeY - 23, info->sizeX - 47, info->sizeY -  4, COL_848484);
	DrawBox(info, info->sizeX - 47, info->sizeY -  3, info->sizeX -  4, info->sizeY -  3, COL_FFFFFF);
	DrawBox(info, info->sizeX -  3, info->sizeY - 24, info->sizeX -  3, info->sizeY -  3, COL_FFFFFF);
}

// ������
void os::Initialize(BootInfo* info)
{
}
