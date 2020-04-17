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
#include <semaphore.h>

struct _fbg *fbg;
sem_t semRender;

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

void postScreenSem()
{
    sem_post(&semRender);
}
void renderDynamicView()
{
    int x = env->getState();
    double inverse_master_gain = 0.0;
    double dec_gain = (master_gain-0.3)*((-1)/(0.3-1));
    double inv_dec_gain = (master_gain-0.3)*((1)/(0.3-1))+1;
    clearScreen();
    for(int i = 0;i < WAVE_TABLE_SIZE;i++)
    {
        inverse_master_gain = abs(master_gain-1.0);
        switch (x)
        {
            case 0:
                currentScreenWavetable[i] = 0.0;
            break;
            case 1://A
                currentScreenWavetable[i] = (master_gain*wave[1][i] + inverse_master_gain*wave[0][i]);
            break;
            case 2://D
                currentScreenWavetable[i] = (dec_gain*wave[1][i] + inv_dec_gain*wave[2][i]);
            break;
            case 3://S
                currentScreenWavetable[i] = wave[2][i];
            break;
            case 4://R
                currentScreenWavetable[i] = (master_gain*wave[2][i] + inverse_master_gain*wave[3][i]);
            break;
        }
        double x = ((currentScreenWavetable[i]+1) * 159.0);
        x = -1 * x + 319;
        x = (int) x;
        setPixel(i,x);
    }
    updateScreen();
    //table2Screen(currentScreenWavetable);
}
void *RenderScreen(void *arg)
{
    sem_init(&semRender, 0, 1);
    while(1)//TODO MAYBE SOMETHING MORE LIKE IF NOT SHUTDOWN
    {
        sem_wait(&semRender);
        if(dynamic_view)
        {
            renderDynamicView();
        }
    }
    return NULL;
}
void fillScreen()
{
}
void endFbGraphics()
{
    fbg_close(fbg);
}
