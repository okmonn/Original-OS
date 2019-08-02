#pragma once

class SegmentDescriptor
{
public:
	// コンストラクタ
	SegmentDescriptor();
	// デストラクタ
	~SegmentDescriptor();

	// パラメータセット
	void SetParam(const unsigned int& limit, const int& base, const int& ar);

private:
	short limit_low;
	short base_low;
	char base_mid;
	char access_right;
	char limit_high;
	char base_high;
};
