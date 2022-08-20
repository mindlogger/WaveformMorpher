// Blur.hpp
#ifndef BLUR_H
#define BLUR_H
#include <stdint.h>
void applyKBlur(double *table, double threshold, double offset);
void applyGaussianBlur();
#endif
