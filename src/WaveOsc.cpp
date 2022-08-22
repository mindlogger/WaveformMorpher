#include "WaveOsc.hpp"
#include "GlobalDefinitions.hpp"
#include "GlobalPreset.hpp"

#include <math.h>
#include <iostream>

using namespace std;
double freq = 440;

double waveOscIndex = 0.0;

double step_size = (double) WAVE_TABLE_SIZE * ((double)440/(double)SAMPLE_RATE);

double interpol = 0.0;

void setNTables()
{
    switch (NTables)
    {
        case 1:
            WaveTable[0] = wave[0];
            WaveTable[1] = wave[0];
            WaveTable[2] = wave[0];
            WaveTable[3] = wave[0];
            WaveTable[4] = wave[0];
        break;
        case 2:
            WaveTable[0] = wave[0];
            WaveTable[1] = wave[0];
            WaveTable[2] = wave[0];
            WaveTable[3] = wave[0];
            WaveTable[4] = wave[1];
        break;
        case 3:
            WaveTable[0] = wave[0];
            WaveTable[1] = wave[1];
            WaveTable[2] = wave[1];
            WaveTable[3] = wave[1];
            WaveTable[4] = wave[2];
        break;
        case 4:
            WaveTable[0] = wave[0];
            WaveTable[1] = wave[1];
            WaveTable[2] = wave[2];
            WaveTable[3] = wave[2];
            WaveTable[4] = wave[3];
        break;
        case 5:
            WaveTable[0] = wave[0];
            WaveTable[1] = wave[1];
            WaveTable[2] = wave[2];
            WaveTable[3] = wave[3];
            WaveTable[4] = wave[4];
        break;
    }
}

void setCueTable()
{
    switch (NTables)
    {
        case 1:
            WaveTable[0] = currentScreenWavetable;
            WaveTable[1] = currentScreenWavetable;
            WaveTable[2] = currentScreenWavetable;
            WaveTable[3] = currentScreenWavetable;
            WaveTable[4] = currentScreenWavetable;
        break;
        case 2:
            if(screenstate == 0)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = wave[0];
                WaveTable[2] = wave[0];
                WaveTable[3] = wave[0];
                WaveTable[4] = currentScreenWavetable;
            }
            else if(screenstate == 1)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = wave[0];
                WaveTable[2] = wave[0];
                WaveTable[3] = wave[0];
                WaveTable[4] = currentScreenWavetable;
            }
        break;
        case 3:
            if(screenstate == 0)
            {
                WaveTable[0] = currentScreenWavetable;
                WaveTable[1] = wave[1];
                WaveTable[2] = wave[1];
                WaveTable[3] = wave[1];
                WaveTable[4] = wave[2];
            }
            else if(screenstate == 1)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = currentScreenWavetable;
                WaveTable[2] = currentScreenWavetable;
                WaveTable[3] = currentScreenWavetable;
                WaveTable[4] = wave[2];
            }
            else if(screenstate == 2)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = wave[1];
                WaveTable[2] = wave[1];
                WaveTable[3] = wave[1];
                WaveTable[4] = currentScreenWavetable;
            }
        break;
        case 4:
            if(screenstate == 0)
            {
                WaveTable[0] = currentScreenWavetable;
                WaveTable[1] = wave[1];
                WaveTable[2] = wave[2];
                WaveTable[3] = wave[2];
                WaveTable[4] = wave[3];
            }
            else if(screenstate == 1)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = currentScreenWavetable;
                WaveTable[2] = wave[2];
                WaveTable[3] = wave[2];
                WaveTable[4] = wave[3];
            }
            else if(screenstate == 2)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = wave[1];
                WaveTable[2] = currentScreenWavetable;
                WaveTable[3] = wave[2];
                WaveTable[4] = wave[3];
            }
            else if(screenstate == 3)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = wave[1];
                WaveTable[2] = wave[2];
                WaveTable[3] = wave[2];
                WaveTable[4] = currentScreenWavetable;
            }
        break;
        case 5:
            if(screenstate == 0)
            {
                WaveTable[0] = currentScreenWavetable;
                WaveTable[1] = wave[1];
                WaveTable[2] = wave[2];
                WaveTable[3] = wave[3];
                WaveTable[4] = wave[4];
            }
            else if(screenstate == 1)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = currentScreenWavetable;
                WaveTable[2] = wave[2];
                WaveTable[3] = wave[3];
                WaveTable[4] = wave[4];
            }
            else if(screenstate == 2)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = wave[1];
                WaveTable[2] = currentScreenWavetable;
                WaveTable[3] = wave[3];
                WaveTable[4] = wave[4];
            }
            else if(screenstate == 3)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = wave[1];
                WaveTable[2] = wave[2];
                WaveTable[3] = currentScreenWavetable;
                WaveTable[4] = wave[4];
            }
            else if(screenstate == 4)
            {
                WaveTable[0] = wave[0];
                WaveTable[1] = wave[1];
                WaveTable[2] = wave[2];
                WaveTable[3] = wave[3];
                WaveTable[4] = currentScreenWavetable;
            }
        break;
    }
}

float getWavetableValue()
{
    step_size = (double) WAVE_TABLE_SIZE * (freq/(double)SAMPLE_RATE);
    double f_x = 0;
    double steigung = 0;
    double nachkomma_x = 0;
    master_gain = envelope->process();
    int envelopeState = envelope->getState();
    double normalizedSustain = knob3Value/4096.0;
    double interpol_a = 0;
    double interpol_b = 0;

    if(envelopeState > 0)
    {
        f_x = WaveTable[envelopeState-1][(int)waveOscIndex];
        steigung = WaveTable[envelopeState-1][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - WaveTable[envelopeState-1][(int)waveOscIndex];
        nachkomma_x = (int)waveOscIndex - waveOscIndex;

        interpol_a = f_x + steigung * nachkomma_x;
    }
    else
    {
        interpol_a = 0;
    }

    f_x = WaveTable[envelopeState][(int)waveOscIndex];
    steigung = WaveTable[envelopeState][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - WaveTable[envelopeState][(int)waveOscIndex];
    nachkomma_x = (int)waveOscIndex - waveOscIndex;

    interpol_b = f_x + steigung * nachkomma_x;

    switch (envelopeState)
    {
        case 0:

        break;
        case 1://A
            interpol = (master_gain*interpol_b + interpol_a*abs(master_gain-1)); //INTERPOL A IST ATTACK; INTERPOL B IST DECAY
        break;
        case 2://D
            {
                double dec_gain = (master_gain-normalizedSustain)*((-1)/(normalizedSustain-1));
                double inv_dec_gain = (master_gain-normalizedSustain)*((1)/(normalizedSustain-1))+1;
                interpol = (dec_gain*interpol_a + interpol_b*inv_dec_gain);

            }
        break;
        case 3://S
            {
                if(loopingFlag)
                {
                    double loop_gain = envelope->getLoopVal();
                    double inv_loop_gain = abs(loop_gain-1.0);
                    interpol = (interpol_a*loop_gain)+(interpol_b*inv_loop_gain);
                }
                else
                {
                    interpol = interpol_a;
                }
            }
        break;
        case 4://R
            {
                f_x = WaveTable[2][(int)waveOscIndex];
                steigung = WaveTable[2][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - WaveTable[2][(int)waveOscIndex];
                nachkomma_x = (int)waveOscIndex - waveOscIndex;

                double interpol_c = f_x + steigung * nachkomma_x;

                double rel_gain = master_gain / normalizedSustain ;

                double inv_rel_gain = abs(rel_gain-1.0);
                interpol = (rel_gain*interpol_c + inv_rel_gain*interpol_b); //INTERPOL B IS w4 INTERPOL C w2
            }
        break;
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

    if(VCAFlag)
        return interpol * master_gain;
    else
        return interpol;
}

void setfreq(double f)
{
    freq = f;
}
