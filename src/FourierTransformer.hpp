//FourierTransformer.hpp
#ifndef FOURIERTRANSFORMER_H
#define FOURIERTRANSFORMER_H
#include <fftw3.h>
void initTransformer(double* waveTable);
double* transForward();
double* transBackward(double* waveTable);
#endif
