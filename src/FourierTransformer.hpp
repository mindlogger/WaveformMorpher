//FourierTransformer.hpp
#include <fftw3.h>
#ifndef FOURIERTRANSFORMER_H
#define FOURIERTRANSFORMER_H
void initTransformer();
void *Transformer(void *args);
void postTransformerSem();
void transForward(int state);
void transBackward(int state);
void endTransformer();
#endif
