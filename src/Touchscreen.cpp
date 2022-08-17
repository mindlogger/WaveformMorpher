#include "Touchscreen.hpp"
#include "FbGraphics.hpp"

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
#include <tuple>
#include <bits/stdc++.h>
#include <string>

#define GLOBALS 1
#include "GlobalDefinitions.hpp"


int fdTouchscreen = -1;//File Descriptor for touch

std::list<std::tuple<size_t, double>> editedPixels;

void signal_callback_handler(int signum)
{

    input_event ev;
    int flag_x = -1;//these correspond to abs values
    int flag_y = -1;
    /*do //wait till btn_touch ev came
    {
        ret = read(in,&ev,sizeof(ev));
    }while((ev.type != 1 && ev.code != 330) );*/
    while(read(fdTouchscreen,&ev,sizeof(ev)) > 0)
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
        else if(ev.type == 1 && ev.value == 0)//BTN_TOUCH OFF
        {
            processlist();
            touch_is_happening_flag = 0;
            table2Screen(currentEditWavetable);
        }
        else if(ev.type == 1 && ev.value == 1)//BTN_TOUCH ON
        {
            touch_is_happening_flag = 1;
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
        out_x = 3860;
    }
    out_y = (16 * (out_y - 160)/185 );//SOME SMART SCALING BUT QUIET CONSERVATIVE
    out_y = abs((int) out_y - 320);
    out_x = (24 * (out_x - 160)/185 );//SOME SMART SCALING BUT QUIET CONSERVATIVE

    //printf("IN Y : %d OUT Y %d || IN X: %d OUT X: %d\n",ev1.value,out_y,ev2.value,out_x);//DEBUG
    if(fourier_flag)
    {
            setPixelOff(out_x);//DELETE OLD PIXEL
            double convertedVal2 = -(out_y/160.0)+1.0;
            currentEditWavetable[out_x] = convertedVal2;//STORE NEW PIXEL
            setPixel(out_x,out_y);//DISPLAY NEW PIXEL
            fft_has_been_touched_flag = 1;
    }
    else
    {
            setPixelOff(out_x);//DELETE OLD PIXEL
            double convertedVal2 = -(out_y/160.0)+1.0;
            setPixel(out_x,out_y);//DISPLAY NEW PIXEL
            editedPixels.push_back({out_x,convertedVal2});//STORE PIXEL TO DRAW LATER
    }
}

///FOR DYNAMIC EVENT ALLOC
/*std::string findEvent()
{
    std::string line;
    std::ifstream myfile;
    myfile.open ("/proc/bus/input/devices");
    do
    {
      if(line.find("Name="stmpe-ts""))
      {

      }
    }while( std::getline(myfile,line) )
    myfile.close();
}*/

void initTouchscreen()
{
    //findEvent();//FOR DYNAMIC EVENT ALLOC
    system("xinput disable stmpe-ts");
    touch_is_happening_flag = 1; //JUST TO NOT TRIGGER THE INT BEFORE TOUCHING THE SCREEN
    signal(SIGUSR1, &signal_callback_handler);
    fdTouchscreen = open("/dev/input/event7",O_RDWR | O_NONBLOCK); //TODO IT CANT BE GUARANTEED THAT THIS IS EV5 FIX DYNAMICALLY ALLOC
    fcntl(fdTouchscreen, F_SETOWN, getpid());
    int oflags = fcntl(fdTouchscreen, F_GETFL);
    fcntl(fdTouchscreen, F_SETFL, oflags | O_ASYNC);
    int k = 0;
    fcntl(fdTouchscreen,F_SETSIG,SIGUSR1);
    int i = getpid();

    printf("My PID: %d\n",i);
    printf("fcntl return: %d\n",k);

    //int flags = 1;
    //ioctl(in, FIOBIO, &flags);

    //raise(SIGUSR1); //FOR DEBUGGIN SENDS SIGNAL TO CURRENT PROCESS
}

void printTouchscreenList()
{
    std::list<std::tuple<size_t, double>>::iterator it = editedPixels.begin();
	while(it != editedPixels.end())
	{
		std::cout<< std::get<0>(*it) <<"  ";
		std::cout<< std::get<1>(*it) <<"  ";
		it++;
	}
	std::cout<<std::endl;
}

void processlist()
{
    std::list<std::tuple<size_t, double>>::iterator it2 = editedPixels.begin();
    std::list<std::tuple<size_t, double>>::iterator it3 = editedPixels.begin();
    it3++;
	while(it3 != editedPixels.end() && it2 != editedPixels.end())
	{
        if(std::get<0>(*it2) == std::get<0>(*it3))
        {
            //std::cout<< "PROC: " << std::get<0>(*it2) <<std::endl;
            it2 = editedPixels.erase(it2);
            it3++;
        }
        else
        {
        it3++;
		it2++;
        }

	}
	//std::cout<<std::endl;

    while(editedPixels.size() > 1)
    {
    double x1 = std::get<0>(editedPixels.front());
    double y1 = std::get<1>(editedPixels.front());
    editedPixels.pop_front();
    double x2 = std::get<0>(editedPixels.front());
    double y2 = std::get<1>(editedPixels.front());
    //std::cout << "SCALED THE FOLLOWING " << x1 << " " << y1 << " "  << x2 << " "  << y2 << " "  << std::endl;
    if(x1<x2)
    {
    drawLine( x1,  y1,  x2,  y2);

    }
    else
    {
    drawLine( x2,  y2,  x1,  y1);
    }
    }
    clearTouchscreenList();
}

void clearTouchscreenList()
{
    editedPixels.clear();
}

void drawLine(double x1, double y1, double x2, double y2)
{
    double m = (y2 - y1) / (x2 - x1);
    //std::cout << "Steigung: " << m <<std::endl;
    currentEditWavetable[(int)x1] = y1;
    for(int c = x1+1;c<x2;c++)
    {
        currentEditWavetable[c] = currentEditWavetable[c-1] + m;
        //std::cout << mainWave[c] << std::endl;
    }
}


void endTouchscreen()
{
    close(fdTouchscreen);
}

