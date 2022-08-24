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

double lastADGainValue = 0.0;

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

uint8_t lastENVState = 0;

float getWavetableValue()
{
    step_size = (double) WAVE_TABLE_SIZE * (freq/(double)SAMPLE_RATE);

    double f_x = 0;
    double steigung = 0;
    double nachkomma_x = 0;

    WMasterGain = WEnv->process();
    AMasterGain = AEnv->process();

    int envelopeState = WEnv->getState();
    double normalizedSustain = 0.5;

    double interpolFirstTable = 0;
    double interpolSecondTable = 0;

    if(envelopeState > 0)
    {
        f_x = WaveTable[envelopeState-1][(int)waveOscIndex];

        steigung = WaveTable[envelopeState-1][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - (WaveTable[envelopeState-1][(int)waveOscIndex]);

        nachkomma_x = (int)waveOscIndex - waveOscIndex;

        interpolFirstTable = (f_x + steigung * abs(nachkomma_x));
        /*if(interpolFirstTable < -1.0 || interpolFirstTable > 1.0)
        {
            std::cout << "STATE " << envelopeState << std::endl;
            std::cout << "master_gain " << master_gain << std::endl;
            std::cout << "f_x " << f_x << std::endl;
            std::cout << "steigung " << steigung << std::endl;
            std::cout << "nachkomma_x " << nachkomma_x << std::endl;
        }*/
    }
    else
    {
        interpolFirstTable = 0;
    }

    f_x = WaveTable[envelopeState][(int)waveOscIndex];

    steigung = WaveTable[envelopeState][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - (WaveTable[envelopeState][(int)waveOscIndex]);

    nachkomma_x = (int)waveOscIndex - waveOscIndex;

    interpolSecondTable = f_x + steigung * abs(nachkomma_x);

    switch (envelopeState)
    {
        case 0:
            lastENVState = 0;
        break;
        case 1://A
            {
                interpol = WMasterGain*interpolSecondTable + interpolFirstTable*abs(WMasterGain-1);
                lastADGainValue = WMasterGain;
                lastENVState = 0;
            }
        break;
        case 2://D
            {
                double dec_gain = (WMasterGain-normalizedSustain)*((-1)/(normalizedSustain-1));
                double inv_dec_gain = abs(dec_gain-1.0);
                interpol = (dec_gain*interpolFirstTable + interpolSecondTable*inv_dec_gain);
                lastADGainValue = dec_gain;
                lastENVState = 1;
            }
        break;
        case 3://S
            {
                if(loopingFlag)
                {
                    //double loop_gain = envelope->getLoopVal();
                    //double inv_loop_gain = abs(loop_gain-1.0);
                    //interpol = (interpol_a*loop_gain)+(interpol_b*inv_loop_gain);
                    interpol = interpolFirstTable; //NO LOOPING FOR NOW
                    lastENVState = 2;
                }
                else
                {
                    interpol = interpolFirstTable;
                    lastENVState = 2;
                }
            }
        break;
        case 4://R
            {
                double interpolSustainTable = 0;
                double rel_gain = WMasterGain;

                switch(lastENVState)
                {
                    case 0: //COMING FROM A-D
                    {
                        f_x = WaveTable[0][(int)waveOscIndex];
                        steigung = WaveTable[0][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - WaveTable[0][(int)waveOscIndex];
                        nachkomma_x = (int)waveOscIndex - waveOscIndex;
                        double interA = f_x + steigung * abs(nachkomma_x);
                        f_x = WaveTable[1][(int)waveOscIndex];
                        steigung = WaveTable[1][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - WaveTable[1][(int)waveOscIndex];
                        nachkomma_x = (int)waveOscIndex - waveOscIndex;
                        double interB = f_x + steigung * abs(nachkomma_x);
                        interpolSustainTable = interB * lastADGainValue + interA * abs(lastADGainValue-1.0);
                        rel_gain = WMasterGain / lastADGainValue;
                    }
                    break;
                    case 1: //COMING FROM D-S
                    {
                        f_x = WaveTable[1][(int)waveOscIndex];
                        steigung = WaveTable[1][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - WaveTable[1][(int)waveOscIndex];
                        nachkomma_x = (int)waveOscIndex - waveOscIndex;
                        double interA = f_x + steigung * abs(nachkomma_x);
                        f_x = WaveTable[2][(int)waveOscIndex];
                        steigung = WaveTable[2][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - WaveTable[2][(int)waveOscIndex];
                        nachkomma_x = (int)waveOscIndex - waveOscIndex;
                        double interB = f_x + steigung * abs(nachkomma_x);
                        interpolSustainTable = interA * lastADGainValue + interB * abs(lastADGainValue-1.0);
                    }
                    break;
                    case 2: //COMING FROM S
                    {
                        f_x = WaveTable[2][(int)waveOscIndex];
                        steigung = WaveTable[2][((int)waveOscIndex + 1) % WAVE_TABLE_SIZE] - WaveTable[2][(int)waveOscIndex];
                        nachkomma_x = (int)waveOscIndex - waveOscIndex;
                        interpolSustainTable = f_x + steigung * abs(nachkomma_x); //THIS IS WHERE LOOPING NEEDS TO GET HANDLED
                    }
                    break;
                }


                double inv_rel_gain = abs(rel_gain-1.0);
                interpol = (rel_gain*interpolSustainTable + inv_rel_gain*interpolSecondTable);
                /*if(interpol < -1.0 || interpol > 1.0)
                {
                    std::cout << "STATE " << envelopeState << std::endl;
                    std::cout << "interpol " << interpol << std::endl;
                    std::cout << "master_gain " << master_gain << std::endl;
                    std::cout << "normalizedSustain " << normalizedSustain << std::endl;
                    std::cout << "interpolFirstTable " << interpolFirstTable << std::endl;
                    std::cout << "interpolSecondTable " << interpolSecondTable << std::endl;
                    std::cout << "interpolSustainTable " << interpolSustainTable << std::endl;
                }*/
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
        return interpol * AMasterGain;
    else
        return interpol;
}

void setfreq(double f)
{
    freq = f;
}
