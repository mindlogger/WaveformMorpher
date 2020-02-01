#include "FourierTransformer.hpp"
#include "GlobalDefinitions.hpp"
#include "math.h"
#include <iostream>

fftw_complex fft_table[WAVE_TABLE_SIZE*2];
double* fft_wave = NULL;
double fft_out[480];

fftw_plan fftForward,fftBackward;
void initTransformer(double* waveTable)
{
    fft_wave = waveTable;
    fftForward = fftw_plan_dft_r2c_1d(WAVE_TABLE_SIZE, fft_wave, fft_table,FFTW_ESTIMATE);
}
double* transForward()
{
    fftw_execute(fftForward);
    for(size_t i = 0;i < 120;i++)
    {
        fft_out[i] = (1/480.0) * sqrt((fft_table[i][0]*fft_table[i][0]) + (fft_table[i][1]*fft_table[i][1]));
    }
    return fft_out;
}
double* transBackward(double* waveTable)
{
    return waveTable;
}
//fft_table = fftw_alloc_complex(WAVE_TABLE_SIZE*2);
//fftw_destroy_plan(p);
