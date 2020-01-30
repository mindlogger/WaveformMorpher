#include "WaveGenerator.hpp"
#include <math.h>

#define M_PI 3.14159265359

void gensinus(float* wave_table,int size)
{
    int i;
	for(i = 0;i < size;i++)
	{
		wave_table[i] = sin(2*M_PI*(1/(float)size)*i);
	}
}

void gensaw(float* wave_table,int size)
{
    int i;
	for(i = 0;i < size;i++)
	{
		wave_table[i] = ((float)i /(float)(size/2) ) - 1.0;
	}
}

void gensqr(float* wave_table,int size)
{
    int i;
	for(i = 0;i < size;i++)
	{
		if(i <= size/2)
		{
			wave_table[i] = -1.0;
		}
		else
		{
			wave_table[i] = 1.0;
		}
	}
}
