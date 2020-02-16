#include "Touchscreen.hpp"
#include "Framebuffer.hpp"

#include <iostream>
#include <fstream>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

#define GLOBALS 1
#include "GlobalDefinitions.hpp"


int in = -1;//File Descriptor for touch
//Color::c = Color::Green;

double* wave_table_touchscreen = NULL;
double* fft_touchscreen = NULL;
void signal_callback_handler(int signum)
{
    input_event ev;
    int flag_x = -1;//these correspond to abs values
    int flag_y = -1;
    /*do //wait till btn_touch ev came
    {
        ret = read(in,&ev,sizeof(ev));
    }while((ev.type != 1 && ev.code != 330) );*/
    while(read(in,&ev,sizeof(ev)) > 0)
    {
        if(ev.type == 3 && ev.code == 0)//IS ABS_X -> Y
        {
            //std::cout << "ABS_X: " << ev.value << std::endl;
            if(flag_y != -1)//there is y value so set both
            {
                //std::cout << "ABS_X: " << ev.value << std::endl;
                flag_x = ev.value;
                toScreen(flag_y,flag_x);//CHANGE FROM X->Y AND Y->X
                flag_y = -1;
                flag_x = -1;
            }
            else //set flag but dont set pixel
            {
                flag_x = ev.value;
            }

        }
        else if(ev.type == 3 && ev.code == 1)//ABS_Y -> X
        {
            //std::cout << "ABS_Y: " << ev.value << std::endl;
            if(flag_x != -1)//there is x value so set both
            {
                flag_y = ev.value;
                toScreen(flag_y,flag_x);//CHANGE FROM X->Y AND Y->X
                flag_x = -1;
                flag_y = -1;
            }
            else //set flag but dont set pixel
            {
                flag_y = ev.value;
            }
        }
    }
}

void toScreen(size_t out_x,size_t out_y)
{
    if(out_y <= 160)
    {
        out_y = 160;
    }
    if(out_x <= 160)
    {
        out_x = 160;
    }
    if(out_y > 3860)
    {
        out_y = 3860;
    }
    if(out_x > 3860)
    {
        out_x = 160;
    }
    out_y = (16 * (out_y - 160)/185 );//SOME SMART SCALING BUT QUIET CONSERVATIVE
    out_y = abs(out_y - 320);
    out_x = (24 * (out_x - 160)/185 );//SOME SMART SCALING BUT QUIET CONSERVATIVE

    //printf("IN Y : %d OUT Y %d || IN X: %d OUT X: %d\n",ev1.value,out_y,ev2.value,out_x);//DEBUG
    if(screenstate == Screenstates::A_W) //TODO CASE SWITCH IN THE FUTURE FOR MORPHING
    {
        int screen_scaling = 5;
        for(int i = 0; i < screen_scaling; i++)
        {
            setPixelOff(out_x + i);//DELETE OLD PIXEL
            double convertedVal2 = -(out_y/160.0)+1.0;
            wave_table_touchscreen[out_x + i] = convertedVal2;//STORE NEW PIXEL
            setPixel(out_x+ i,out_y);//DISPLAY NEW PIXEL
        }
    }
    else
    {
        int screen_scaling = 1;
        for(int i = 0; i < screen_scaling; i++)
        {
            setPixelOff(out_x + i);//DELETE OLD PIXEL
            double convertedVal2 = -(out_y/160.0)+1.0;
            fft_touchscreen[out_x + i] = convertedVal2;//STORE NEW PIXEL
            setPixel(out_x+ i,out_y);//DISPLAY NEW PIXEL
        }
    }
}

void initTouchscreen(double* screenWave,double* screenFFT)
{
    wave_table_touchscreen = screenWave;
    fft_touchscreen = screenFFT;
    screenstate = Screenstates::A_W;
    signal(SIGUSR1, &signal_callback_handler);
    in = open("/dev/input/event3",O_RDWR | O_NONBLOCK); //TODO IT CANT BE GUARANTEED THAT THIS IS EV5 FIX DYNAMICALLY ALLOC
    fcntl(in, F_SETOWN, getpid());
    int oflags = fcntl(in, F_GETFL);
    fcntl(in, F_SETFL, oflags | O_ASYNC);
    int k = 0;
    fcntl(in,F_SETSIG,SIGUSR1);
    int i = getpid();

    printf("My PID: %d\n",i);
    printf("fcntl return: %d\n",k);

    //int flags = 1;
    //ioctl(in, FIOBIO, &flags);




    //raise(SIGUSR1); //FOR DEBUGGIN SENDS SIGNAL TO CURRENT PROCESS
}


