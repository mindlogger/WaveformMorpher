#include "WaveGenerator.hpp"
#include <math.h>

void genSin(double* waveTable,int N)
{
    int i;
	for(i = 0;i < N;i++)
	{
		waveTable[i] = sin(2*M_PI*(1/(double)N)*i);
	}
}

void genSaw(double* waveTable,int N)
{
    int i;
	for(i = 0;i < N;i++)
	{
		waveTable[i] = ((double)i /(double)(N/2) ) - 1.0;
	}
}

void genSqr(double* waveTable,int N)
{
    int i;
	for(i = 0;i < N;i++)
	{
		if(i <= N/2)
		{
			waveTable[i] = -1.0;
		}
		else
		{
			waveTable[i] = 1.0;
		}
	}
}
