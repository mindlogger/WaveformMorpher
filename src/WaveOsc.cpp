#include "WaveOsc.hpp"
#include "GlobalDefinitions.hpp"
#include <math.h>

double freq = 440;

double waveOscIndex = 0.0;

double step_size = (double) WAVE_TABLE_SIZE * ((double)440/(double)SAMPLE_RATE);

double interpol = 0.0;

float getWavetableValue()
{
    step_size = (double) WAVE_TABLE_SIZE * (freq/(double)SAMPLE_RATE);
    double f_x = 0;
    double steigung = 0;
    double nachkomma_x = 0;
    master_gain = env->process();
    int x = env->getState();
    double interpol_a = 0;
    double interpol_b = 0;

    f_x = wave[x-1][(int)waveOscIndex];
    steigung = wave[x-1][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - wave[x-1][(int)waveOscIndex];
    nachkomma_x = (int)waveOscIndex - waveOscIndex;

    interpol_a = f_x + steigung * nachkomma_x;

    f_x = wave[x][(int)waveOscIndex];
    steigung = wave[x][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - wave[x][(int)waveOscIndex];
    nachkomma_x = (int)waveOscIndex - waveOscIndex;

    interpol_b = f_x + steigung * nachkomma_x;

    if(x == env_decay)
    {
    double dec_gain = (master_gain-0.3)*((-1)/(0.3-1));
    double inv_dec_gain = (master_gain-0.3)*((1)/(0.3-1))+1;
    interpol = (dec_gain*interpol_a + interpol_b*inv_dec_gain);
    }
    else
    {
    if(x == env_sustain)
    {
    interpol = (interpol_a);
    }
    else
    {
    interpol = (master_gain*interpol_b + interpol_a*abs(master_gain-1));
    }

    }

    waveOscIndex = waveOscIndex + step_size;

    if(waveOscIndex >= WAVE_TABLE_SIZE-1)
    {
        waveOscIndex = (waveOscIndex - WAVE_TABLE_SIZE - 1); // start at new overflow value
        if(waveOscIndex < 0) //machts iwie stabiler
        {
            waveOscIndex = waveOscIndex * (-1);
        }
    }

    return interpol * master_gain;
}

void setfreq(double f)
{
    freq = f;
}
