#include "FourierTransformer.hpp"
#include "GlobalDefinitions.hpp"
#include "math.h"
#include <iostream>
#include <cstring>

fftw_complex *fft_table_complex;

double* fft_wave = NULL;
double* ifft_wave = NULL;

double fft_out[WAVE_TABLE_SIZE];
double ifft_out[WAVE_TABLE_SIZE];

double phase_table[WAVE_TABLE_SIZE];

fftw_plan fftForward,fftBackward;

void initTransformer(double* main_wave,double* main_fft)
{
    fft_table_complex = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*WAVE_TABLE_SIZE/2);
    ifft_wave = main_fft; //make var locally available
    fft_wave = main_wave; //make var locally available
    fftForward = fftw_plan_dft_r2c_1d(WAVE_TABLE_SIZE, fft_wave, fft_table_complex,FFTW_ESTIMATE);
    fftBackward = fftw_plan_dft_c2r_1d(WAVE_TABLE_SIZE, fft_table_complex, ifft_out,FFTW_ESTIMATE);
}
double* transForward()
{
    fftw_execute(fftForward);
    for(size_t i = 0;i < WAVE_TABLE_SIZE;i++)
    {
        fft_out[i] = (1.0/WAVE_TABLE_SIZE) * sqrt((fft_table_complex[i][0]*fft_table_complex[i][0]) + (fft_table_complex[i][1]*fft_table_complex[i][1])); //CALCULATING MAGNITUDE

        phase_table[i] = atan2((1.0/WAVE_TABLE_SIZE) * fft_table_complex[i][1],(1.0/WAVE_TABLE_SIZE) * fft_table_complex[i][0]); //CALCULATING PHASE
        //std::cout << "Nummer " << i << ": " << fft_table[i][0] << " " << fft_table[i][1] << std::endl; //DEBUG COMPLEX
        //std::cout << "Nummer " << i << ": " << fft_out[i] << std::endl; //DEBUG MAGNITUDE
        fft_out[i] = (fft_out[i]*3.8) - 1.0;//THIS STEP IS JUST TO CONFORM TO -1 TO 1 SCREEN SCALING
    }

    return fft_out;
}
double* transBackward()
{
    fftw_complex *old_d;
    old_d = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*WAVE_TABLE_SIZE/2);
    memcpy(old_d,fft_table_complex,sizeof(fftw_complex) * WAVE_TABLE_SIZE/2);
    size_t counter = 0;
    for(size_t i = 0;i < WAVE_TABLE_SIZE;i = i+2)//STEP IN 2 CUS COMPLEX ARRAY IS N/2
    {
        ifft_wave[counter] = (ifft_wave[i] + 1.0)/3.8; //BACKSCALING FROM THE SCREEN
        counter++;
    }
    for(size_t k = 0;k < WAVE_TABLE_SIZE/2;k++)//PROPER SCALING
    {
        old_d[k][0] = (1.0/WAVE_TABLE_SIZE) * old_d[k][0];
        old_d[k][1] = (1.0/WAVE_TABLE_SIZE) * old_d[k][1];
    }
    for(size_t j = 0;j < WAVE_TABLE_SIZE/2;j++)
    {
        fft_table_complex[j][0] = ifft_wave[j] * cos(phase_table[j]);//CALC REAL
        fft_table_complex[j][1] = ifft_wave[j] * sin(phase_table[j]);//CALC COMPLEX
        std::cout << "NUMBER: " << j << std::endl;
        //std::cout << "MAG: " << ifft_wave[j] << std::endl; //DEBUG MAGNITUDE
        std::cout << "OLD: " << old_d[j][0] << " " << old_d[j][1] << std::endl;
        std::cout << "NEW: " << fft_table_complex[j][0] << " " << fft_table_complex[j][1] << std::endl;
    }/*
    for(size_t j = 0;j < (WAVE_TABLE_SIZE/2) +1;j++)
    {
        fft_table[j][0] = 0.0;//CALC REAL
        fft_table[j][1] = 0.0;//CALC COMPLEX
    }
    fft_table[1][1] = -0.5; //DEBUG
    */

    fftw_execute(fftBackward);
    /*for(size_t i = 0;i < WAVE_TABLE_SIZE;i++)//PROPER SCALING
    {
        std::cout << "NUMBER: " << i << std::endl;
        std::cout << ifft_out[i] << std::endl;
    }*/
    return ifft_out;
}
//fft_table = fftw_alloc_complex(WAVE_TABLE_SIZE*2);
//fftw_destroy_plan(p);
