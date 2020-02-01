#include "AudioOut.hpp"
#include "Framebuffer.hpp"
#include "Touchscreen.hpp"
#include "WaveOsc.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"
#include "GlobalDefinitions.hpp"
#include <fftw3.h>

#include "iostream"



using namespace std;

double screenWave[WAVE_TABLE_SIZE]; //y = 0..150..300

int main()
{
    setupTimer();
    genSqr(screenWave,WAVE_TABLE_SIZE);
    initFramebuffer(screenWave);
    initAudio(screenWave);
    initTouchscreen(screenWave);
    initMidi();
    initTransformer(screenWave);
    double* test = transForward();
    std::cout << test[1] << std::endl;
    table2Screen(test);
    while(true)
    {
        //loopedtouch();
    }
    return 0;
}

