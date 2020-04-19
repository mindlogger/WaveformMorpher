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

