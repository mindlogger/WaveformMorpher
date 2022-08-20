#include "AudioOut.hpp"
#include "FbGraphics.hpp"
#include "Touchscreen.hpp"
#include "WaveOsc.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"
#include "ADSR.hpp"
#include "UI.hpp"
#include "ButtonActions.hpp"

#include "GlobalDefinitions.hpp"

#include <pigpio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

int handle; //Handle for the I2C bridge
sem_t semUI;

void callbackSW1(int gpio, int level, uint32_t tick) //L/S
{
    if(level)
    {
        if(shift_flag)
        {
            SW1ShiftEvent(tick, 7);
        }
        else
        {
            SW1Event(tick, 1);
        }    }
}
void callbackSW2(int gpio, int level, uint32_t tick) //w/N
{
    if(level)
    {
        if(shift_flag)
        {
            SW2ShiftEvent(tick, 8);
        }
        else
        {
            SW2Event(tick, 2);
        }
    }
}
void callbackSW3(int gpio, int level, uint32_t tick) // S
{
    if(level)
    {
        if(shift_flag)
        {
            SW3ShiftEvent(tick, 9);
        }
        else
        {
            SW3Event(tick, 3);
        }
    }
}
void callbackSW4(int gpio, int level, uint32_t tick) //?
{
    if(level)
    {
        if(shift_flag)
        {
            SW4ShiftEvent(tick, 10);
        }
        else
        {
            SW4Event(tick, 4);
        }
    }
}
void callbackSW5(int gpio, int level, uint32_t tick) //C/P
{
    if(level)
    {
        if(shift_flag)
        {
            SW5ShiftEvent(tick, 11);
        }
        else
        {
            SW5Event(tick, 5);
        }
    }
}
void callbackSW6(int gpio, int level, uint32_t tick) //F/I
{
    if(level)
    {
        if(shift_flag)
        {
            SW6ShiftEvent(tick, 12);
        }
        else
        {
            SW6Event(tick, 6);
        }
    }
}
/*
SW1 GPIO 4 LOAD/STOR
SW2 GPIO 17 W/N
SW3 GPIO 27 PS/GS
SW4 GPIO 22 SHIFT
SW5 GPIO 14 CP/PS
SW6 GPIO 15 FFT/PRESETWAVE
*/
#define GLITCH_THRSHLD 200
void setupUI()
{
        assignMainActions();
        gpioInitialise();
        //gpioSetPullUpDown(0,PI_PUD_UP);
        //gpioSetPullUpDown(1,PI_PUD_UP);
        handle = i2cOpen(1,0x33,0); //0b0110011
        char setupbyte = 0b11110110; //SETUP BIT
        i2cWriteDevice(handle,&setupbyte,1);

        gpioGlitchFilter(4, GLITCH_THRSHLD);
        gpioGlitchFilter(17,GLITCH_THRSHLD);
        gpioGlitchFilter(27,GLITCH_THRSHLD);
        gpioGlitchFilter(22,GLITCH_THRSHLD);
        gpioGlitchFilter(14,GLITCH_THRSHLD);
        gpioGlitchFilter(15,GLITCH_THRSHLD);
        gpioSetAlertFunc(4,  callbackSW1);
        gpioSetAlertFunc(17, callbackSW2);
        gpioSetAlertFunc(27, callbackSW3);
        gpioSetAlertFunc(22, callbackSW4);
        gpioSetAlertFunc(14, callbackSW5);
        gpioSetAlertFunc(15, callbackSW6);

        pthread_t ui_t;
        pthread_create(&ui_t,NULL,handle_ui,NULL);
}
void postUISem()
{
    sem_post(&semUI);
}
unsigned int readADC(unsigned int pin)// pin:0-7;ret:0 - 4096
{
    if(pin < 8)
    {
    char receive[2] = {0b0000000,0b0000000};
    int t = (pin << 1);
    char receivewbyte = 0b01100001 | t; //CONFIG BIT
    i2cWriteDevice(handle,&receivewbyte,1);
    i2cReadDevice(handle,receive,2);
    receive[0] = receive[0] ^ 0b11110000;
    return (int) receive[1] | ((int) receive[0] << 8);
    }
    else
    {
    return 0;
    }
}
void getADCValues()//TODO MAYBE SOME NICER SCALING HERE?
{
    att_v = readADC(0);
    dec_v = readADC(1);
    sus_v = readADC(2);
    rel_v = readADC(3);
    loop_v = readADC(7);;
    envelope->setAttackRate(((5*att_v/4096.0) + 0.01) * SAMPLE_RATE);
    envelope->setDecayRate(((5*dec_v/4096.0) + 0.01) * SAMPLE_RATE);
    envelope->setSustainLevel(sus_v/4096.0);
    envelope->setReleaseRate(((5*rel_v/4096.0) + 0.01) * SAMPLE_RATE);
    double adc_7 = loop_v;
    if(adc_7 > 3700)
    {
        envelope->setPingPong(0);
    }
    else
    {
        envelope->setPingPong(1);
        envelope->setLoopRate(/*((5*(adc_7+0.0001)/4095.0) + 0.05)*/ 3 * SAMPLE_RATE); //DEBUG SHIT FOR NOW
    }
    if(readADC(5) > 1028)
    {
        shift_flag = 1;
    }
    else
    {
        shift_flag = 0;
    }
    if(gpioRead(23))
    {
    //envelope->gate(false);
    }
    else
    {
    //envelope->gate(true);
    }
    /*double inbetwiener = readADC(4);
    double v_offset = -0.000003;
    double par1 = 9.0092307623 - (0.002024291498+v_offset) * inbetwiener;
    double par2 = 65.406391325149 * pow(2,par1);
    setfreq(par2);*/
    /*std::cout << "Ports:" << std::endl;
    std::cout << readADC(4) << std::endl;
    std::cout << readADC(5) << std::endl;
    std::cout << readADC(6) << std::endl;
    std::cout << readADC(7) << std::endl << std::endl;*/

    //envelope->setLoopRate(((5*readADC(X)/4096.0) + 0.01) * SAMPLE_RATE);
    //if(readADC(X) == 0) SET LOOP OFF
}
void *handle_ui(void *arg)
{
    sem_init(&semUI, 0, 1);
    while(n_shutdown_flag)//TODO MAYBE SOMETHING MORE LIKE IF NOT SHUTDOWN
    {
        sem_wait(&semUI);
        getADCValues();
    }
    gpioTerminate();
    return NULL;
}
void *handle_input(void *arg)
{
        char input = '0';
        int input_int = 0;
        input_int = getchar();
        input = (char) input_int;
        if(input_int != -1)
        {
        switch(input)
        {
            case 'e' :
            {
            n_shutdown_flag = 0;
            }
            break;
            case 'f' :
            {
                if(!fourier_flag)
                {
                cout << "forward transform" << endl;
                currentEditWavetable = fft[screenstate];
                transForward(screenstate);
                renderScreen();
                fourier_flag = 1;
                }
            }
            break;
            case 'b' :
            {
                if(fourier_flag)
                {
                cout << "backwards transform" << endl;
                currentEditWavetable = wave[screenstate];
                transBackward(screenstate);
                renderScreen();
                fourier_flag = 0;
                }
            }
            break;
            case 'r' :
            {
                cout << "rounding" << endl;

            }
            case 'g' :
            {
                cout << (int) screenstate << endl;

            }break;
            case 'd' :
            {
                if(dynamic_view)
                {
                    cout << "toggled dynamic mode OFF" << endl;
                    renderScreen();
                    dynamic_view = 0;
                }
                else
                {
                    cout << "toggled dynamic mode ON" << endl;
                    dynamic_view = 1;
                }

            }break;
            case 's' :
            {
                genSin(currentEditWavetable);
                renderScreen();
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;

            }break;
            case 'q' :
            {
                genSqr(currentEditWavetable);
                renderScreen();
                screenTable2Continuous();
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;

            }break;
            case 'w' :
            {
                genSaw(currentEditWavetable);
                renderScreen();
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;
            }break;
            case '<' :
            {
                genSil(currentEditWavetable);
                renderScreen();
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;
            }break;
            case '1' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[0];
                renderScreen();
                cout << "Attack Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[0];
                renderScreen();
                cout << "Attack Wave" << endl;
                }
                screenstate = A;
            }break;
            case '2' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[1];
                renderScreen();
                cout << "Decay Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[1];
                renderScreen();
                cout << "Decay Wave" << endl;
                }
                screenstate = D;
            }break;
            case '3' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[2];
                renderScreen();
                cout << "Sustain Start Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[2];
                renderScreen();
                cout << "Sustain Start Wave" << endl;
                }
                screenstate = SS;
            }break;
            case '4' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[3];
                renderScreen();
                cout << "Sustain End Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[3];
                renderScreen();
                cout << "Sustain End Wave" << endl;
                }
                screenstate = SE;
            }break;
            case '5' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[4];
                renderScreen();
                cout << "Release Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[4];
                renderScreen();
                cout << "Release Wave" << endl;
                }
                screenstate = R;
            }break;
            default :
                {
                //cout << "\nBad Input: " << input << endl;
                }
            break;
        }
        }
    return NULL;
}

