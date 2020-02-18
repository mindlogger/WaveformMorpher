// Touchscreen.hpp
#include <sys/types.h>
#include <fftw3.h>
#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H
void initTouchscreen();
void signal_callback_handler(int signum);
void toScreen(size_t x,size_t y);
#endif
