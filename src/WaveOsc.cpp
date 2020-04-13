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
    switch (x)
    {
    case env_idle:
         f_x = 0;
         steigung = 0;
         nachkomma_x = 0;
        break;
    case env_attack:
             f_x = wave[0][(int)waveOscIndex];
             steigung = wave[0][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - wave[0][(int)waveOscIndex];
             nachkomma_x = (int)waveOscIndex - waveOscIndex;

             interpol_a = f_x + steigung * nachkomma_x;

             f_x = wave[1][(int)waveOscIndex];
             steigung = wave[1][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - wave[1][(int)waveOscIndex];
             nachkomma_x = (int)waveOscIndex - waveOscIndex;

             interpol_b = f_x + steigung * nachkomma_x;

            interpol = (master_gain*interpol_b + interpol_a*abs(master_gain-1));

        break;
    case env_decay:
             f_x = wave[1][(int)waveOscIndex];
             steigung = wave[1][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - wave[1][(int)waveOscIndex];
             nachkomma_x = (int)waveOscIndex - waveOscIndex;
                 interpol = f_x + steigung * nachkomma_x;
        break;
    case env_sustain:
             f_x = wave[2][(int)waveOscIndex];
             steigung = wave[2][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - wave[2][(int)waveOscIndex];
             nachkomma_x = (int)waveOscIndex - waveOscIndex;
                 interpol = f_x + steigung * nachkomma_x;
        break;
    case env_release:
             f_x = wave[3][(int)waveOscIndex];
             steigung = wave[3][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - wave[3][(int)waveOscIndex];
             nachkomma_x = (int)waveOscIndex - waveOscIndex;
                 interpol = f_x + steigung * nachkomma_x;
        break;
    }


    /*double f_x = audioOutWavetable[(int)waveOscIndex];
    double steigung = audioOutWavetable[((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - audioOutWavetable[(int)waveOscIndex];
    double nachkomma_x = (int)waveOscIndex - waveOscIndex;*/

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
