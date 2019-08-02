#pragma once
#include "Typedef.h"

class BootInfo;
class SegmentDescriptor;
class GateDescriptor;

namespace os
{
	// sprintf�̃I���W�i��
	void Print(char* str, const char* fmt, ...);

	// �J�[�\���`��
	void DrawCursor(BootInfo* info, const int& x, const int& y, const Color& color);

	// �{�b�N�X�`��
	void DrawBox(BootInfo* info, const int& x1, const int& y1, const int& x2, const int& y2, const Color& color);

	// �t�H���g�`��
	void DrawFont(BootInfo* info, const unsigned char* font, const int& x, const int& y, const Color& color);

	// ������`��
	void DrawString(BootInfo* info, const char* str, const int& x, const int& y, const Color& color);

	// �L�q�q�̏�����
	void InitDescriptor(SegmentDescriptor* gdt, GateDescriptor* idt);

	// PIC�̏�����
	void InitPIC(void);

	// �J���[�p���b�g�̏�����
	void InitPalette(const unsigned int& start, const unsigned int& end, const unsigned char* rgb);

	// �E�B���h�E���C�A�E�g�̏�����
	void InitLayout(BootInfo* info);

	// ������
	void Initialize(BootInfo* info);
}
