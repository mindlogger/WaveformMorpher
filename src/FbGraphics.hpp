// FbGraphics.hpp
#ifndef FBGRAPHICS_H
#define FBGRAPHICS_H
void setPixel(int x, int y);
void setPixelOff(int x, int y);
void setPixelOff(int x);
void clearScreen();
void updateScreen();
void postScreenSem();
void renderDynamicView();
void *RenderScreen(void *arg);
void initFbGraphics();
void table2Screen(double* wave_table);
void screenTable2Continuous();
void fillScreen();
void endFbGraphics();
#endif
