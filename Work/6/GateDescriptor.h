#pragma once

class GateDescriptor
{
public:
	// �R���X�g���N�^
	GateDescriptor();
	// �f�X�g���N�^
	~GateDescriptor();

	// �p�����[�^�Z�b�g
	void SetParam(const int& offset, const int& select, const int& ar);

private:
	short offset_low;
	short selector;
	char dw_count;
	char access_right;
	short offset_high;
};
