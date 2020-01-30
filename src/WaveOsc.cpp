#include "WaveOsc.hpp"

#include <math.h>

#define WAVE_TABLE_SIZE (480)
#define SAMPLE_RATE (48000)

float freq = 440;

float table_index_out = 0;

float step_size = (float) WAVE_TABLE_SIZE * ((float)440/(float)SAMPLE_RATE);

float interpol = 0.0;

float getWavetableValue(float* wave_table)
{
    step_size = (float) WAVE_TABLE_SIZE * ((float)freq/(float)SAMPLE_RATE);

    // copy sine value to dma output buffer
    float f_x = wave_table[(int)table_index_out];
    float steigung = wave_table[((int)table_index_out + 1) % WAVE_TABLE_SIZE] - wave_table[(int)table_index_out];
    float nachkomma_x = (int)table_index_out - table_index_out;

    interpol = f_x + steigung * nachkomma_x;

    table_index_out = table_index_out + step_size;

    if(table_index_out >= WAVE_TABLE_SIZE-1)
    {
        table_index_out = table_index_out - WAVE_TABLE_SIZE-1; // start at new overflow value
        if(table_index_out < 0) //machts iwie stabiler
        {
            table_index_out = table_index_out * -1;
        }
    }

    return interpol;
}

void setfreq(float f)
{
    freq = f;
}
