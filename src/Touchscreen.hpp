// Touchscreen.hpp
#include <sys/types.h>
#include <fftw3.h>
#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H
void initTouchscreen();
void signal_callback_handler(int signum);
void toScreen(size_t x,size_t y);
void processlist();
void printTouchscreenList();
void clearTouchscreenList();
void drawLine(double x1, double y1, double x2, double y2);
void endTouchscreen();
#endif
