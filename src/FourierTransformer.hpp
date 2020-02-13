//FourierTransformer.hpp
#include <fftw3.h>
#ifndef FOURIERTRANSFORMER_H
#define FOURIERTRANSFORMER_H
void initTransformer(double* waveTable,double* fftTable);
double* transForward();
double* transBackward();
#endif
