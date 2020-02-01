#include "WaveOsc.hpp"
#include "GlobalDefinitions.hpp"
#include <math.h>

double freq = 440;

double table_index_out = 0;

double step_size = (double) WAVE_TABLE_SIZE * ((double)440/(double)SAMPLE_RATE);

double interpol = 0.0;

double getWavetableValue(double* wave_table)
{
    step_size = (double) WAVE_TABLE_SIZE * (freq/(double)SAMPLE_RATE);

    // copy sine value to dma output buffer
    double f_x = wave_table[(int)table_index_out];
    double steigung = wave_table[((int)table_index_out + 1) % WAVE_TABLE_SIZE] - wave_table[(int)table_index_out];
    double nachkomma_x = (int)table_index_out - table_index_out;

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

void setfreq(double f)
{
    freq = f;
}
