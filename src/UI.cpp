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

#include "GlobalDefinitions.hpp"

#include <pigpio.h>
#include <cstring>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

int handle; //Handle for the I2C bridge
sem_t semUI;

void setupUI()
{
        gpioInitialise();
        handle = i2cOpen(1,0x33,0); //0b0110011
        char setupbyte = 0b11110110; //SETUP BIT
        i2cWriteDevice(handle,&setupbyte,1);
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
void getADCValues()
{
    envelope->setAttackRate(((5*readADC(0)/4096.0) + 0.01) * SAMPLE_RATE);
    envelope->setDecayRate(((5*readADC(1)/4096.0) + 0.01) * SAMPLE_RATE);
    sus_v = (readADC(2)/4096.0);
    envelope->setSustainLevel(sus_v);
    envelope->setReleaseRate(((5*readADC(3)/4096.0) + 0.01) * SAMPLE_RATE);
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
        char input = '2';
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
                table2Screen(currentEditWavetable);
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
                table2Screen(currentEditWavetable);
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
                table2Screen(currentEditWavetable);
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;

            }break;
            case 'q' :
            {
                genSqr(currentEditWavetable);
                table2Screen(currentEditWavetable);
                screenTable2Continuous();
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;

            }break;
            case 'w' :
            {
                genSaw(currentEditWavetable);
                table2Screen(currentEditWavetable);
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;
            }break;
            case '<' :
            {
                genSil(currentEditWavetable);
                table2Screen(currentEditWavetable);
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;
            }break;
            case '1' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[0];
                table2Screen(currentEditWavetable);
                cout << "Attack Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[0];
                table2Screen(currentEditWavetable);
                cout << "Attack Wave" << endl;
                }
                screenstate = A;
            }break;
            case '2' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[1];
                table2Screen(currentEditWavetable);
                cout << "Decay Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[1];
                table2Screen(currentEditWavetable);
                cout << "Decay Wave" << endl;
                }
                screenstate = D;
            }break;
            case '3' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[2];
                table2Screen(currentEditWavetable);
                cout << "Sustain Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[2];
                table2Screen(currentEditWavetable);
                cout << "Sustain Wave" << endl;
                }
                screenstate = SS;
            }break;
            case '4' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[3];
                table2Screen(currentEditWavetable);
                cout << "Release Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[3];
                table2Screen(currentEditWavetable);
                cout << "Release Wave" << endl;
                }
                screenstate = SE;
            }break;
            case '5' :
            {
                if(fourier_flag)
                {
                currentEditWavetable = fft[4];
                table2Screen(currentEditWavetable);
                cout << "??? Spectrum" << endl;
                }
                else
                {
                currentEditWavetable = wave[4];
                table2Screen(currentEditWavetable);
                cout << "??? Wave" << endl;
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

