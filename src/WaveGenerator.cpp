#include "WaveGenerator.hpp"
#include "GlobalDefinitions.hpp"
#include <math.h>

void genSin(double* waveTable)
{
	for(int i = 0;i < WAVE_TABLE_SIZE;i++)
	{
		waveTable[i] = sin(2*M_PI*(1/(double)WAVE_TABLE_SIZE)*i);
	}
}

void genSaw(double* waveTable)
{
    int i;
	for(i = 0;i < WAVE_TABLE_SIZE;i++)
	{
		waveTable[i] = ((double)i /(double)(WAVE_TABLE_SIZE/2) ) - 1.0;
	}
}

void genSqr(double* waveTable)
{
    int i;
	for(i = 0;i < WAVE_TABLE_SIZE;i++)
	{
		if(i < WAVE_TABLE_SIZE/2)
		{
			waveTable[i] = -1.0;
		}
		else
		{
			waveTable[i] = 1.0;
		}
	}
}
