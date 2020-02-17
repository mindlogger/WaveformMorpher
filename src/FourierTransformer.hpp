//FourierTransformer.hpp
#include <fftw3.h>
#ifndef FOURIERTRANSFORMER_H
#define FOURIERTRANSFORMER_H
void initTransformer(double* main_wave,double* main_fft);
double* transForward();
double* transBackward();
#endif
