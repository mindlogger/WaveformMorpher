// Touchscreen.hpp
#include <sys/types.h>
#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H
void initTouchscreen(double *screenWave);
void signal_callback_handler(int signum);
void toScreen(size_t x,size_t y);
#endif
