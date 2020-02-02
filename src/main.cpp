#include "AudioOut.hpp"
#include "Framebuffer.hpp"
#include "Touchscreen.hpp"
#include "WaveOsc.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"
#include "GlobalDefinitions.hpp"

#include "iostream"



using namespace std;

double mainWave[WAVE_TABLE_SIZE]; //y = 0..150..300

int main()
{
    //setupTimer();
    genSqr(mainWave,WAVE_TABLE_SIZE);
    initFramebuffer(mainWave);
    initAudio(mainWave);
    initTouchscreen(mainWave);
    initMidi();
    initTransformer(mainWave);

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
                double* test = transForward();
                table2Screen(test);
                std::cout << test[1] << std::endl;
                break ;
            }
            case 'b' :
            {
                cout << "backwards transform" << endl;
                table2Screen(mainWave);
                break;
            }
            default : cout << "\nBad Input. Must be f or b" ;
        }
        //loopedtouch();
    }
    return 0;
}

