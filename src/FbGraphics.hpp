// FbGraphics.hpp
#ifndef FBGRAPHICS_H
#define FBGRAPHICS_H

#include <string>

uint16_t calcCenterOfText(std::string text);
void addText(std::string text, int x, int y, int timeout);
void setLine(int x1, int y1,int x2, int y2);
void setPixel(int x, int y);
void setPixelOff(int x, int y);
void setPixelOff(int x);
void drawBrowsingArrows();
void drawSaveIcon();
void clearScreen();
void commitScreenBuffer();
void updateScreen();
void renderScreen();
void postScreenSem();
void renderDynamicView();
void *RenderScreen(void *arg);
void initFbGraphics();
void table2Screen(double* wave_table);
void fillScreen();
void endFbGraphics();
#endif
