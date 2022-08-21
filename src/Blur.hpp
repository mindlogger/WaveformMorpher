// Blur.hpp
#ifndef BLUR_H
#define BLUR_H
#include <stdint.h>
void applyKBlurForward(double *sourceTable, double *destinationTable, double threshold, double offset, uint16_t window);
void applyKBlurBackward(double *sourceTable, double *destinationTable, double threshold, double offset, uint16_t window);
void applyMedianBlur(double *sourceTable, double *destinationTable, uint16_t rounds);
void applyGaussianBlur(double *sourceTable, double *destinationTable);
#endif
