#include "AudioOut.hpp"
#include "FbGraphics.hpp"
#include "Touchscreen.hpp"
#include "WaveOsc.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"
#include "ADSR.hpp"

#include "GlobalDefinitions.hpp"

#include <cstring>
#include <iostream>

using namespace std;

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
            case 'f' :
            {
                //currentScreenWavetable = mainFFT;
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

            }break;
            case 'b' :
            {
                //currentScreenWavetable = mainWave;
                cout << "backwards transform" << endl;
                double* ifft_out_prescale = transBackward();
                //table2Screen(ifft_out_prescale);
                memcpy(mainWave,ifft_out_prescale,sizeof(double) * WAVE_TABLE_SIZE);
                //screenTable2Continuous();
                screenstate = Screenstates::A_W;

                for(size_t i = 0; i < WAVE_TABLE_SIZE-1;i++) //THIS SHOULD NOT NEED TO BE HERE
                {
                    mainWave[i] = (mainWave[i] + mainWave[i+1]) / 2.0;
                }
                table2Screen(mainWave);
                screenTable2Continuous();


            }break;
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
            case '1' :
            {
                currentEditWavetable = wave[0];
                table2Screen(currentEditWavetable);
                //TODO SET SCREENSTATE TO WAVE
                cout << "Attack Wave" << endl;

            }break;
            case '2' :
            {
                currentEditWavetable = wave[1];
                table2Screen(currentEditWavetable);
                //TODO SET SCREENSTATE TO WAVE
                cout << "Decay Wave" << endl;

            }break;
            case '3' :
            {
                currentEditWavetable = wave[2];
                table2Screen(currentEditWavetable);
                //TODO SET SCREENSTATE TO WAVE
                cout << "Sustain Wave" << endl;

            }break;
            case '4' :
            {
                currentEditWavetable = wave[3];
                table2Screen(currentEditWavetable);
                //TODO SET SCREENSTATE TO WAVE
                cout << "Release Wave" << endl;

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

