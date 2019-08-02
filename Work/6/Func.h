#pragma once
#include "Typedef.h"

class BootInfo;
class SegmentDescriptor;
class GateDescriptor;

namespace os
{
	// sprintfのオリジナル
	void Print(char* str, const char* fmt, ...);

	// カーソル描画
	void DrawCursor(BootInfo* info, const int& x, const int& y, const Color& color);

	// ボックス描画
	void DrawBox(BootInfo* info, const int& x1, const int& y1, const int& x2, const int& y2, const Color& color);

	// フォント描画
	void DrawFont(BootInfo* info, const unsigned char* font, const int& x, const int& y, const Color& color);

	// 文字列描画
	void DrawString(BootInfo* info, const char* str, const int& x, const int& y, const Color& color);

	// 記述子の初期化
	void InitDescriptor(SegmentDescriptor* gdt, GateDescriptor* idt);

	// PICの初期化
	void InitPIC(void);

	// カラーパレットの初期化
	void InitPalette(const unsigned int& start, const unsigned int& end, const unsigned char* rgb);

	// ウィンドウレイアウトの初期化
	void InitLayout(BootInfo* info);

	// 初期化
	void Initialize(BootInfo* info);
}
