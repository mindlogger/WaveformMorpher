#include "WaveGenerator.hpp"
#include "GlobalDefinitions.hpp"
#include <math.h>

void genSin()
{
    int i;
	for(i = 0;i < WAVE_TABLE_SIZE;i++)
	{
		mainWave[i] = sin(2*M_PI*(1.0/(double)WAVE_TABLE_SIZE)*i);
	}
}

void genSaw()
{
    int i;
	for(i = 0;i < WAVE_TABLE_SIZE;i++)
	{
		mainWave[i] = ((double)i /(double)(WAVE_TABLE_SIZE/2.0) ) - 1.0;
	}
}

void genSqr()
{
    int i;
	for(i = 0;i < WAVE_TABLE_SIZE;i++)
	{
		if(i < WAVE_TABLE_SIZE/2)
		{
			mainWave[i] = -1.0;
		}
		else if(i == WAVE_TABLE_SIZE/2)
		{
            mainWave[i] = 0.0;
		}
		else
		{
			mainWave[i] = 1.0;
		}
	}
}
