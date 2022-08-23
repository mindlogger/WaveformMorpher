#include "FbGraphics.hpp"
#include "GlobalDefinitions.hpp"
#include "UIRenderer.hpp"
#include "GlobalPreset.hpp"

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

sem_t semRender;

using namespace std;

void textTimerHandler(union sigval timer_data)
{
    renderScreen();
}

uint16_t calcCenterOfText(std::string text)
{
    return 240 - (text.length() * 8);
}

void addText(std::string text, int x, int y, int timeout)
{
    //clearScreen();
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

void setLine(int x1, int y1,int x2, int y2)
{
    fbg_line(fbg,x1,y1,x2,y2,255,255,255);
}

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
    switch(uiState)
    {
        case EditView:
            renderEditView();
        break;
        case DynamicView:
            renderPatchSettings();
        break;
        case PatchSettings:
            renderPatchSettings();
        break;
        case GlobalSettings:
            renderGlobalSettings();
        break;
        case Load:
            renderLoad();
        break;
        case Store:
            renderStore();
        break;
        case InsertWave:
            renderInsertWave();
        break;
        case HiddenMode:
            renderHiddenMode();
        break;
        case BlurMode:
            renderBlurMode();
        break;
        case FilterMode:
            renderFilterMode();
        break;
        case AttenuateMode:
            renderAttenuateMode();
        break;
        case UserInsertWave:
            renderUserInsertWave();
        break;
    }}

void initFbGraphics()
{
    char *path = strdup("/dev/fb1"); //THIS IS HERE TO AVOID WARNINGS
    fbg = fbg_setup(path, 0); // you can also directly use fbg_init(); for "/dev/fb0", last argument mean that will not use page flipping mechanism  for double buffering (it is actually slower on some devices!)
    fbg_clear(fbg, 0); // can also be replaced by fbg_fill(fbg, 0, 0, 0);
    fbg_fill(fbg,255,255,255);
    bb_font_img = fbg_loadImage(fbg, "/home/pi/WaveformMorpherFonts/default_font.png");
    bbfont = fbg_createFont(fbg, bb_font_img, 16, 16, 33);
    pthread_t render_t;
    pthread_create(&render_t,NULL,RenderScreen,NULL);
}

void table2Screen(double* wave_table)
{
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
        if(fGP.Visual.continous && i > 0)
        {
            double x2 = ((wave_table[i-1]+1) * 159.0);
            x2 = -1 * x2 + 319;
            x2 = round(x2);
            setLine(i,x,i,x2);
        }
        //std::cout << "setpixel " << i << ": " << x << std::endl; //DEBUG
    }
    commitScreenBuffer();
}

void postScreenSem()
{
    sem_post(&semRender);
}

void renderDynamicView()
{
    int envelopeState = envelope->getState();
    if(envelopeState == 0) //ONLY RENDER STUFF IF PLAYING
    {
        renderScreen();
    }
    else
    {
        clearScreen();
        double (*working_wavetable)[480];
        if(fourier_flag)
        {
            working_wavetable = fft;
        }
        else
        {
            working_wavetable = wave;
        }

        double normalizedSustain = knob3Value/4096.0;
        double inverse_master_gain = abs(master_gain-1.0);
        double dec_gain = (master_gain-normalizedSustain)*((-1)/(normalizedSustain-1));
        double inv_dec_gain = (master_gain-normalizedSustain)*((1)/(normalizedSustain-1))+1;
        double rel_gain = master_gain / normalizedSustain;
        double inv_rel_gain = abs(rel_gain-1.0);
        double loop_gain = envelope->getLoopVal();
        double inv_loop_gain = abs(loop_gain-1.0);

        for(int i = 0;i < WAVE_TABLE_SIZE;i++)
        {
            switch (envelopeState)
            {
                case 1://A
                    fbg_write(fbg, "A.", 4, 2);
                    currentScreenWavetable[i] = (master_gain*working_wavetable[1][i] + inverse_master_gain*working_wavetable[0][i]);
                break;
                case 2://D
                    fbg_write(fbg, "D.", 4, 2);
                    currentScreenWavetable[i] = (dec_gain*working_wavetable[1][i] + inv_dec_gain*working_wavetable[2][i]);
                break;
                case 3://S
                    fbg_write(fbg, "S.", 4, 2);
                    if(loopingFlag)
                    {
                        currentScreenWavetable[i] = loop_gain * working_wavetable[2][i] + inv_loop_gain * working_wavetable[3][i];
                    }
                    else
                    {
                        currentScreenWavetable[i] = working_wavetable[2][i];
                    }
                break;
                case 4://R
                    fbg_write(fbg, "R.", 4, 2);
                    if(loopingFlag)
                    {
                        currentScreenWavetable[i] = (rel_gain*working_wavetable[3][i] + inv_rel_gain*working_wavetable[4][i]); //TODO FIX THIS AT SOME POINT AND FETCH A PROPER WAVETABLE IN TIME
                    }
                    else
                    {
                        currentScreenWavetable[i] = (rel_gain*working_wavetable[2][i] + inv_rel_gain*working_wavetable[4][i]);
                    }

                break;
            }
        }
        table2Screen(currentScreenWavetable);
    }
}

void *RenderScreen(void *arg)
{
    sem_init(&semRender, 0, 1);
    while(n_shutdown_flag)
    {
        sem_wait(&semRender);

        //cout << envelope->getState() << endl; //GREAT PLACE TO DEBUG STUFF :)

        if(dynamic_view)
        {
            renderDynamicView();
        }
        else if(uiState == BlurMode)
        {
            renderBlurMode();
        }
        else if(uiState == AttenuateMode)
        {
            renderAttenuateMode();
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
