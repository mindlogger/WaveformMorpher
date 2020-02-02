#include "WaveOsc.hpp"
#include "GlobalDefinitions.hpp"
#include <math.h>

double freq = 440;

double waveOscIndex = 0.0;

double step_size = (double) WAVE_TABLE_SIZE * ((double)440/(double)SAMPLE_RATE);

double interpol = 0.0;

float getWavetableValue(double* wave_table)
{
    step_size = (double) WAVE_TABLE_SIZE * (freq/(double)SAMPLE_RATE);

    double f_x = wave_table[(int)waveOscIndex];
    double steigung = wave_table[((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - wave_table[(int)waveOscIndex];
    double nachkomma_x = (int)waveOscIndex - waveOscIndex;

    interpol = f_x + steigung * nachkomma_x;

    waveOscIndex = waveOscIndex + step_size;

    if(waveOscIndex >= WAVE_TABLE_SIZE-1)
    {
        waveOscIndex = (waveOscIndex - WAVE_TABLE_SIZE - 1); // start at new overflow value
        if(waveOscIndex < 0) //machts iwie stabiler
        {
            waveOscIndex = waveOscIndex * (-1);
        }
    }

    return interpol;
}

void setfreq(double f)
{
    freq = f;
}
