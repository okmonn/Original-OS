/*-----インクルード-----*/
#include "Declaration.h"

// 絶対値
double Abs(int integer)
{
	double tmp = integer;
	if(tmp < 0)
	{
		tmp *= -1;
	}
	return tmp;
}

// iのnum乗
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
