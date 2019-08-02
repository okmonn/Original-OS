#pragma once

class GateDescriptor
{
public:
	// コンストラクタ
	GateDescriptor();
	// デストラクタ
	~GateDescriptor();

	// パラメータセット
	void SetParam(const int& offset, const int& select, const int& ar);

private:
	short offset_low;
	short selector;
	char dw_count;
	char access_right;
	short offset_high;
};
