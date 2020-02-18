//FourierTransformer.hpp
#include <fftw3.h>
#ifndef FOURIERTRANSFORMER_H
#define FOURIERTRANSFORMER_H
void initTransformer();
double* transForward();
double* transBackward();
#endif
