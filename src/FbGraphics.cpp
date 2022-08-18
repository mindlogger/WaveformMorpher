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
#include <semaphore.h>
#include <pthread.h>
#include <string>

struct _fbg *fbg;
struct _fbg_img *bb_font_img;
struct _fbg_font *bbfont;

sem_t semRender;

void textTimerHandler(union sigval timer_data)
{
    clearScreen();
    updateScreen();
}

void addText(std::string text, int x, int y, int timeout)
{
    clearScreen();
    fbg_write(fbg, &text[0], x, y);
    updateScreen();

    struct sigevent timer_signal_event;
    timer_t timer;

    struct itimerspec timer_period;

    timer_signal_event.sigev_notify = SIGEV_THREAD;
    timer_signal_event.sigev_notify_function = textTimerHandler;       // This function will be called when timer expires
    timer_signal_event.sigev_notify_attributes = NULL;
    timer_create(CLOCK_MONOTONIC, &timer_signal_event, &timer);

    timer_period.it_value.tv_sec = timeout;                                   // 1 second timer
    timer_period.it_value.tv_nsec = 0;                                  // no nano-seconds
    timer_period.it_interval.tv_sec = 0;                                // non-repeating timer
    timer_period.it_interval.tv_nsec = 0;

    timer_settime(timer, 0, &timer_period, NULL);
}

void addRedrawTimeout(int timeout)
{}

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
void commitScreenBuffer()
{
    fbg_flip(fbg);
    fbg_draw(fbg);
}

void updateScreen()
{
    table2Screen(currentEditWavetable);
}

void renderScreen()
{
    clearScreen();
    table2Screen(currentEditWavetable);}


void initFbGraphics()
{
    char *path = strdup("/dev/fb1"); //THIS IS HERE TO AVOID WARNINGS
    fbg = fbg_setup(path, 0); // you can also directly use fbg_init(); for "/dev/fb0", last argument mean that will not use page flipping mechanism  for double buffering (it is actually slower on some devices!)
    fbg_clear(fbg, 0); // can also be replaced by fbg_fill(fbg, 0, 0, 0);
    fbg_fill(fbg,255,255,255);
    bb_font_img = fbg_loadImage(fbg, "img/16px_roboto_mono_bitmap_font.png");
    bbfont = fbg_createFont(fbg, bb_font_img, 16, 16, 33);
    pthread_t render_t;
    pthread_create(&render_t,NULL,RenderScreen,NULL);
}

void table2Screen(double* wave_table)
{
    if(settingstate == PS || settingstate == GS)
    {
        switch(settingstate)
        {
        case PS:
        fbg_write(fbg, "Patch Settings", 140, 15);
        commitScreenBuffer();
        return;
        break;
        case GS:
        fbg_write(fbg, "Global Settings", 140, 15);
        commitScreenBuffer();
        return;
        break;
        }
    }
    else
    {
        switch(screenstate)
        {
            case A:
            fbg_write(fbg, "A", 4, 2);
            break;
            case D:
            fbg_write(fbg, "D", 4, 2);
            break;
            case SS:
            fbg_write(fbg, "SS", 4, 2);
            break;
            case SE:
            fbg_write(fbg, "SE", 4, 2);
            break;
            case R:
            fbg_write(fbg, "R", 4, 2);
            break;
        }

        for(int i = 0; i<WAVE_TABLE_SIZE; i++)
        {
            double x = ((wave_table[i]+1) * 159.0);
            x = -1 * x + 319;
            x = round(x);
            if(x < 0) //ONLY FOR DEBUGING REASONS
            {
                //std::cout << "err:table2Screen:tosmall: " << x << " at pos " << i << std::endl; //DEBUG
                x = 0;
            }
            if(x > 319) //ONLY FOR DEBUGING REASONS
            {
                //std::cout << "err:table2Screen:tolarge: " << x << " at pos " << i << std::endl; //DEBUG
                x = 319;
            }
            setPixel(i,x);
            //std::cout << "setpixel " << i << ": " << x << std::endl; //DEBUG
        }
    }
    commitScreenBuffer();
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
    double idle = 0.0;
    double (*working_wavetable)[480];
    if(fourier_flag)
    {
    idle = -1.0;
    working_wavetable = fft;
    }
    else
    {
    idle = 0.0;
    working_wavetable = wave;
    }
    int x = envelope->getState();
    double inverse_master_gain = abs(master_gain-1.0);
    double dec_gain = (master_gain-sus_v)*((-1)/(sus_v-1));
    double inv_dec_gain = (master_gain-sus_v)*((1)/(sus_v-1))+1;
    double rel_gain = master_gain*(1.0/sus_v);
    double inv_rel_gain = abs(master_gain*(1.0/sus_v)-1.0);
    double loop_gain = envelope->getLoopVal();
    double inv_loop_gain = abs(loop_gain-1.0);

    for(int i = 0;i < WAVE_TABLE_SIZE;i++)
    {

        switch (x)
        {
            case 0:
                currentScreenWavetable[i] = idle;
            break;
            case 1://A
                currentScreenWavetable[i] = (master_gain*working_wavetable[1][i] + inverse_master_gain*working_wavetable[0][i]);
            break;
            case 2://D
                currentScreenWavetable[i] = (dec_gain*working_wavetable[1][i] + inv_dec_gain*working_wavetable[2][i]);
            break;
            case 3://S
                currentScreenWavetable[i] = loop_gain * working_wavetable[2][i] + inv_loop_gain * working_wavetable[3][i];
            break;
            case 4://R
                currentScreenWavetable[i] = (rel_gain*working_wavetable[3][i] + inv_rel_gain*working_wavetable[4][i]);
            break;
        }
    }
    table2Screen(currentScreenWavetable);
}
void *RenderScreen(void *arg)
{
    sem_init(&semRender, 0, 1);
    while(n_shutdown_flag)
    {
        sem_wait(&semRender);
        if(dynamic_view)
        {
            renderDynamicView();
        }
        else
        {
        /*
        char xa[4];
        xa[0] = 'X';
        xa[1] = '1';
        xa[2] = 'B';
        xa[3] = '\0';
        sprintf(xa, "%d", (int) screenstate);
        fbg_write(fbg, xa , 25, 25);
        //std::cout << xa[1] << std::endl;
        fbg_draw(fbg);
        fbg_flip(fbg);*/
        }
    }
    return NULL;
}
void fillScreen()
{
}
void endFbGraphics()
{
    fbg_freeImage(bb_font_img);
    fbg_freeFont(bbfont);
    fbg_close(fbg);
}
