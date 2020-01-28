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


int in = -1;//File Descriptor for touch
int* wave_table_touchscreen = NULL;
void signal_callback_handler(int signum)
{
    input_event ev1;//ABS_X -> Y
    input_event ev2;//ABS_Y -> X
    read(in,&ev1,sizeof(ev1));
    if(ev1.type == 3 && ev1.code == 0)
    {
        read(in,&ev2,sizeof(ev2));
        if(ev1.type == 3 && ev2.code == 1)//ONLY DO SMTH IF NEXT IS Y CODE
        {
            if(ev1.value <= 160)
            {
                ev1.value = 160;
            }
            if(ev2.value <= 160)
            {
                ev2.value = 160;
            }
            if(ev1.value > 3860)
            {
                ev1.value = 3860;
            }
            if(ev2.value > 3860)
            {
                ev2.value = 160;
            }
            //while(ev2.code != 1 || ev2.type != 3 || ev2.value < 205); //WHILE WE GET A Y EVENT FOR THE X VALUE
            int out_y,out_x;

            out_y = (16 * (ev1.value - 160)/185 );//SOME SMART SCALING BUT QUIET CONSERVATIVE
            out_y = abs(out_y - 320);
            out_x = (24 * (ev2.value - 160)/185 );//SOME SMART SCALING BUT QUIET CONSERVATIVE

            printf("IN Y : %d OUT Y %d || IN X: %d\ OUT X: %d\n",ev1.value,out_y,ev2.value,out_x);//DEBUG

            int screen_scaling = 4;
            for(int i = 0; i < screen_scaling; i++)
            {
                setPixelOff(out_x + i,wave_table_touchscreen[out_x+ i]);//DELETE OLD PIXEL
                wave_table_touchscreen[out_x+ i] = out_y;//STORE NEW PIXEL
                setPixel(out_x+ i,out_y);//DISPLAY NEW PIXEL
            }
        }
    }
}

void loopedtouch()
{
    input_event ev1;//ABS_X -> Y
    input_event ev2;//ABS_Y -> X
    read(in,&ev1,sizeof(ev1));
    if(ev1.type == 3 && ev1.code == 0)
    {
        read(in,&ev2,sizeof(ev2));
        if(ev2.code == 1)//ONLY DO SMTH IF NEXT IS Y CODE
        {
            if(ev1.value <= 160)
            {
                ev1.value = 160;
            }
            if(ev2.value <= 160)
            {
                ev2.value = 160;
            }
            if(ev1.value > 3860)
            {
                ev1.value = 3860;
            }
            if(ev2.value > 3860)
            {
                ev2.value = 160;
            }
            //while(ev2.code != 1 || ev2.type != 3 || ev2.value < 205); //WHILE WE GET A Y EVENT FOR THE X VALUE
            int out_y,out_x;

            out_y = (16 * (ev1.value - 160)/185 );//SOME SMART SCALING BUT QUIET CONSERVATIVE
            out_y = abs(out_y - 320);
            out_x = (24 * (ev2.value - 160)/185 );//SOME SMART SCALING BUT QUIET CONSERVATIVE

            printf("IN Y : %d OUT Y %d || IN X: %d\ OUT X: %d\n",ev1.value,out_y,ev2.value,out_x);//DEBUG

            int screen_scaling = 4;
            for(int i = 0; i < screen_scaling; i++)
            {
                setPixelOff(out_x + i,wave_table_touchscreen[out_x+ i]);//DELETE OLD PIXEL
                wave_table_touchscreen[out_x+ i] = out_y;//STORE NEW PIXEL
                setPixel(out_x+ i,out_y);//DISPLAY NEW PIXEL
            }
        }
    }
}
int initTouchscreen(int* screen_wave)
{
    //signal(SIGUSR1, &signal_callback_handler);
    in = open("/dev/input/event3",O_RDONLY); //TODO IT CANT BE GUARANTEED THAT THIS IS EV5 FIX DYNAMICALLY ALLOC
    /*fcntl(in, F_SETOWN, getpid());
    int oflags = fcntl(in, F_GETFL);
    fcntl(in, F_SETFL, oflags | O_ASYNC);
    int k = 0;
    fcntl(in,F_SETSIG,SIGUSR1);
    int i = getpid();

    printf("My PID: %d\n",i);
    printf("fcntl return: %d\n",k);
    */
    wave_table_touchscreen = screen_wave;

    //raise(SIGUSR1); //FOR DEBUGGIN SENDS SIGNAL TO CURRENT PROCESS
}


