#include "AudioOut.hpp"
#include "Framebuffer.hpp"
#include "Touchscreen.hpp"
#include "WaveOsc.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"

#include "GlobalDefinitions.hpp"

#include <cstring>
#include <iostream>



using namespace std;

double mainWave[WAVE_TABLE_SIZE]; //y = 0..150..300
double mainFFT[WAVE_TABLE_SIZE];

int main()
{
    setupTimer();
    genSin(mainWave,WAVE_TABLE_SIZE);
    initFramebuffer(mainWave);
    //initAudio(mainWave);
    initTouchscreen(mainWave,mainFFT);
    //initMidi();

    initTransformer(mainWave,mainFFT);
    screenTable2Continuous();
    while(true)
    {

        char input;
        std::cout << "Enter f for forward or b for backward: ";
        cin >> input;
        switch(input)
        {
            case 'f' :
            {
                cout << "forward transform" << endl;
                double* fft_out_prescale = transForward();
                double fft_out_postscale[WAVE_TABLE_SIZE];
                for(size_t i = 0; i < WAVE_TABLE_SIZE;i++)
                {
                    fft_out_postscale[i] = (fft_out_prescale[i/2]);//scale without any interpoaltion because FFT is only N/2 in length
                }
                table2Screen(fft_out_postscale);
                memcpy(mainFFT,fft_out_postscale,sizeof(double) * WAVE_TABLE_SIZE); //make globaly available
                screenTable2Continuous();
                screenstate = Screenstates::A_F;
                break ;
            }
            case 'b' :
            {
                cout << "backwards transform" << endl;
                double* ifft_out_prescale = transBackward();
                table2Screen(ifft_out_prescale);
                memcpy(mainWave,ifft_out_prescale,sizeof(double) * WAVE_TABLE_SIZE);
                screenTable2Continuous();
                screenstate = Screenstates::A_W;
                break;
            }
            case 'r' :
            {
                cout << "rounding" << endl;
                for(size_t i = 0; i < WAVE_TABLE_SIZE-1;i++)
                {
                    mainWave[i] = (mainWave[i] + mainWave[i+1]) / 2.0;
                }
                table2Screen(mainWave);
                screenTable2Continuous();
                break;
            }
            case 'g' :
            {
                cout << (int) screenstate << endl;
                break;
            }
            case 's' :
            {
                genSin(mainWave,WAVE_TABLE_SIZE);
                table2Screen(mainWave);
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;
                break;
            }
            case 'q' :
            {
                genSqr(mainWave,WAVE_TABLE_SIZE);
                table2Screen(mainWave);
                screenTable2Continuous();
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;
                break;
            }
            case 'w' :
            {
                genSaw(mainWave,WAVE_TABLE_SIZE);
                table2Screen(mainWave);
                //TODO SET SCREENSTATE TO WAVE
                cout << (int) screenstate << endl;
                break;
            }
            default : cout << "\nBad Input. Must be f,b or r" ;
        }
    }
    return 0;
}

