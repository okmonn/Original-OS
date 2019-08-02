#include "GateDescriptor.h"

// コンストラクタ
GateDescriptor::GateDescriptor()
{
	offset_low = selector = 0;
	dw_count = access_right = 0;
	offset_high = 0;
}

// デストラクタ
GateDescriptor::~GateDescriptor()
{
}

// パラメータセット
void GateDescriptor::SetParam(const int& offset, const int& select, const int& ar)
{
	offset_low   = offset & 0xffff;
	selector     = select;
	dw_count     = (ar >> 8) & 0xff;
	access_right = ar & 0xff;
	offset_high  = (offset >> 16) & 0xffff;
}
