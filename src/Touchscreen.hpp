// Touchscreen.hpp
#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H
int initTouchscreen(int *wave_table);
void signal_callback_handler(int signum);
void loopedtouch(void);
#endif
