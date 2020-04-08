#include "FbGraphics.hpp"
#include "GlobalDefinitions.hpp"

extern "C"
{
#include "fbgraphics.h"
}
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <cstring>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>

struct _fbg *fbg;

void setPixel(int x, int y)
{
    fbg_pixel(fbg,x,y,255,255,255);
}
void setPixelOff(int x, int y)
{
    fbg_pixel(fbg,x,y,0,0,0);
}
void setPixelOff(int x)
{
    fbg_line(fbg,x,0,x,320,0,0,0);
}
void clearScreen()
{
    fbg_clear(fbg, 0);
}
void updateScreen()
{
    fbg_flip(fbg);
    fbg_draw(fbg);
}
void initFbGraphics()
{
    fbg = fbg_setup("/dev/fb1", 0); // you can also directly use fbg_init(); for "/dev/fb0", last argument mean that will not use page flipping mechanism  for double buffering (it is actually slower on some devices!)
    fbg_clear(fbg, 0); // can also be replaced by fbg_fill(fbg, 0, 0, 0);
    fbg_fill(fbg,255,255,255);
}
void table2Screen(double* wave_table)
{
    clearScreen();
    for(int i = 0; i<WAVE_TABLE_SIZE; i++)
    {
        double x = ((wave_table[i]+1) * 159.0);
        x = -1 * x + 319;
        x = round(x);
        if(x < 0) //ONLY FOR DEBUGING REASONS
        {
            std::cout << "err:table2Screen:tosmall: " << x << std::endl; //DEBUG
            x = 0;
        }
        if(x > 319) //ONLY FOR DEBUGING REASONS
        {
            std::cout << "err:table2Screen:tolarge: " << x << std::endl; //DEBUG
            x = 319;
        }
        setPixel(i,x);

        //std::cout << "setpixel " << i << ": " << x << std::endl; //DEBUG
        //wave_table_framebuffer[i] = x;
    }
    updateScreen();

}
void screenTable2Continuous()
{
}
void fillScreen()
{
}
void endFbGraphics()
{
    fbg_close(fbg);
}
