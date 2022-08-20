// Blur.hpp
#ifndef BLUR_H
#define BLUR_H
#include <stdint.h>
void applyKBlur(double *sourceTable, double *destinationTable, double threshold, double offset);
void applyGaussianBlur(double *sourceTable, double *destinationTable);
#endif
