#include "SegmentDescriptor.h"
// コンストラクタ
SegmentDescriptor::SegmentDescriptor()
{
	limit_low = base_low = 0;
	base_mid = access_right = 0;
	limit_high = base_high = 0;
}

// デストラクタ
SegmentDescriptor::~SegmentDescriptor()
{
}

// パラメータセット
void SegmentDescriptor::SetParam(const unsigned int& limit, const int& base, const int& ar)
{
	unsigned int lim = limit;
	int access = ar;
	if (lim > 0xfffff)
	{
		access |= 0x8000; //G_bit = 1
		lim /= 0x1000;
	}

	limit_low    = lim & 0xffff;
	base_low     = base & 0xffff;
	base_mid     = (base >> 16) & 0xff;
	access_right = access & 0xff;
	limit_high   = ((lim >> 16) & 0x0f) | ((access >> 8) & 0xf0);
	base_high    = (base >> 24) & 0xff;
}
