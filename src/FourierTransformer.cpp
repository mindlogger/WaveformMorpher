#include "FourierTransformer.hpp"
#include "GlobalDefinitions.hpp"
#include "math.h"
#include <iostream>

fftw_complex *fft_table;// [WAVE_TABLE_SIZE*2];
double* fft_wave = NULL;
double fft_out[WAVE_TABLE_SIZE];
double* ifft_wave = NULL;
double ifft_out[WAVE_TABLE_SIZE];

fftw_plan fftForward,fftBackward;

void initTransformer(double* waveTable,fftw_complex* fftTable)
{
    fft_wave = waveTable;
    fft_table = fftTable;
    fftForward = fftw_plan_dft_r2c_1d(WAVE_TABLE_SIZE, fft_wave, fft_table,FFTW_ESTIMATE);
    fftBackward = fftw_plan_dft_c2r_1d(WAVE_TABLE_SIZE, fft_table, ifft_out,FFTW_ESTIMATE);
}
double* transForward()
{
    fftw_execute(fftForward);
    for(size_t i = 0;i < WAVE_TABLE_SIZE;i++)
    {
        fft_out[i] = (1.0/WAVE_TABLE_SIZE) * sqrt((fft_table[i][0]*fft_table[i][0]) + (fft_table[i][1]*fft_table[i][1]));//CALCULATING MAGNITUDE
        fft_out[i] = sqrt(fft_out[i]); //MAKING IT MORE INTRESTING TO VIEW
        //std::cout << "Nummer " << i << ": " << fft_out[i] << std::endl; //DEBUG
        fft_out[i] = fft_out[i]*2 - 1.0;//THIS STEP IS JUST TO CONFORM TO -1 TO 1 SCREEN SCALING
    }
    return fft_out;
}
double* transBackward()
{
    for(size_t i = 0;i < WAVE_TABLE_SIZE;i++)//PROPER SCALING
    {
        fft_table[i][0] = (1.0/WAVE_TABLE_SIZE) *fft_table[i][0];
        fft_table[i][1] = (1.0/WAVE_TABLE_SIZE) *fft_table[i][1];
    }
    fftw_execute(fftBackward);
    return ifft_out;
}
//fft_table = fftw_alloc_complex(WAVE_TABLE_SIZE*2);
//fftw_destroy_plan(p);
