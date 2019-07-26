/*-----�C���N���[�h-----*/
#include "Declaration.h"

// ��Βl
double Abs(int integer)
{
	double tmp = integer;
	if(tmp < 0)
	{
		tmp *= -1;
	}
	return tmp;
}

// i��num��
double Pow(unsigned int integer, int num)
{
	double tmp = 1;
	while(num != 0)
	{
		if(num > 0)
		{
			tmp *= integer;
			--num;
		}
		else
		{
			tmp /= integer;
			++num;
		}
	}
	return tmp;
}
