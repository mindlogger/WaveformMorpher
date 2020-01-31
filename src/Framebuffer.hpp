// Framebuffer.hpp
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
void setPixel(int x, int y);
void setPixelOff(int x, int y);
void setPixelOff(int x);
void clearScreen();
void initFramebuffer(float* wave_table);
void table2Screen(float* wave_table);
#endif
