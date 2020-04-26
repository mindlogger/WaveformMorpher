#include "FourierTransformer.hpp"
#include "GlobalDefinitions.hpp"
#include "math.h"
#include <iostream>
#include <cstring>
#include <semaphore.h>
#include <pthread.h>

#define FFT_MAG_SCALAR 5.0

fftw_complex *fft_table_complex[5];

double fft_out[5][WAVE_TABLE_SIZE];
double ifft_out[5][WAVE_TABLE_SIZE];

double magTable[5][WAVE_TABLE_SIZE];
double phaseTable[5][WAVE_TABLE_SIZE];

fftw_plan fftForward[5],fftBackward[5];

sem_t semTranformer;

void initTransformer()
{
    for(int i = 0;i < 5; i++)
    {
        fft_table_complex[i] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*WAVE_TABLE_SIZE/2);
        fftForward[i] = fftw_plan_dft_r2c_1d(WAVE_TABLE_SIZE, wave[i], fft_table_complex[i],FFTW_ESTIMATE);
        fftBackward[i] = fftw_plan_dft_c2r_1d(WAVE_TABLE_SIZE, fft_table_complex[i], wave[i],FFTW_ESTIMATE);
    }
    pthread_t transformer_t;
    pthread_create(&transformer_t,NULL,Transformer,NULL);
}
void *Transformer(void *args)
{
    sem_init(&semTranformer, 0, 1);
    while(n_shutdown_flag)
    {
    if(fourier_flag && fft_has_been_touched_flag)
    {
    fft_has_been_touched_flag = 0;
    transBackward(screenstate);
    }
    }
}
void postTransformerSem()
{
    sem_post(&semTranformer);
}
void endTransformer()
{
    for(int i = 0;i < 5; i++)
    {
    fftw_destroy_plan(fftForward[i]);
    fftw_destroy_plan(fftBackward[i]);
    fftw_free(fft_table_complex[i]);
    }
}
void transForward(int state)
{
    fftw_execute(fftForward[state]);
    for(size_t i = 0;i < WAVE_TABLE_SIZE;i++)
    {
        magTable[state][i] = (1.0/WAVE_TABLE_SIZE) * sqrt((fft_table_complex[state][i][0]*fft_table_complex[state][i][0]) + (fft_table_complex[state][i][1]*fft_table_complex[state][i][1])); //CALCULATING MAGNITUDE
        phaseTable[state][i] = atan2((1.0/WAVE_TABLE_SIZE) * fft_table_complex[state][i][1],(1.0/WAVE_TABLE_SIZE) * fft_table_complex[state][i][0]); //CALCULATING PHASE
        //std::cout << "Nummer " << i << ": " << magTable[state][i] << " " << phaseTable[state][i] << std::endl; //DEBUG COMPLEX POLAR


        fft[state][i] = (log(magTable[state][i/2]+1.0)*FFT_MAG_SCALAR) - 1.0;//SCREEN SCALING
        //std::cout << "NUMMER " << i << ": " << fft[state][i] << std::endl;
        //std::cout << "Nummer " << i << ": " << fft[state][i] << std::endl; //DEBUG SCREEN_MAGNITUDE
    }
}
void transBackward(int state)
{
    fftw_complex *old_d; //DEBUG START
    old_d = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*WAVE_TABLE_SIZE/2);
    for(size_t k = 0;k < WAVE_TABLE_SIZE/2;k++)//PROPER SCALING
    {
        old_d[k][0] = (1.0/WAVE_TABLE_SIZE) * old_d[k][0];
        old_d[k][1] = (1.0/WAVE_TABLE_SIZE) * old_d[k][1];
    }
    memcpy(old_d,fft_table_complex[state],sizeof(fftw_complex) * WAVE_TABLE_SIZE/2);//DEBUG END

    double mainFFT[WAVE_TABLE_SIZE];
    size_t counter = 0;
    for(size_t i = 0;i < WAVE_TABLE_SIZE;i = i+2)//STEP IN 2 CUS COMPLEX ARRAY IS N/2
    {
        mainFFT[counter] = (exp((fft[state][i]/FFT_MAG_SCALAR) + (1.0/FFT_MAG_SCALAR)) - 1.0); //BACKSCALING FROM THE SCREEN
        counter++;
    }

    for(size_t j = 0;j < WAVE_TABLE_SIZE/2;j++)
    {
        fft_table_complex[state][j][0] = mainFFT[j] * cos(phaseTable[state][j]);//CALC REAL
        fft_table_complex[state][j][1] = mainFFT[j] * sin(phaseTable[state][j]);//CALC COMPLEX
        /*
        std::cout << "NUMBER: " << j << std::endl;
        std::cout << "MAGT: " << magTable[state][j] << std::endl; //DEBUG MAGNITUDE
        std::cout << "MAGG: " << mainFFT[j] << std::endl; //DEBUG MAGNITUDE
        std::cout << "OLD: " << old_d[j][0] << " " << old_d[j][1] << std::endl;
        std::cout << "NEW: " << fft_table_complex[state][j][0] << " " << fft_table_complex[state][j][1] << std::endl;
        */
    }
    fftw_free(old_d);
    fftw_execute(fftBackward[state]);
    for(size_t i = 0; i < WAVE_TABLE_SIZE-1;i++) //TODO THIS SHOULD NOT NEED TO BE HERE
    {
        wave[screenstate][i] = (wave[screenstate][i] + wave[screenstate][i+1]) / 2.0;
    }
}

