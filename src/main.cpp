#include "AudioOut.hpp"
#include "FbGraphics.hpp"
#include "Touchscreen.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"
#include "ADSR.hpp"

#include "GlobalDefinitions.hpp"

#include <signal.h>

void main_init()
{
    env = new ADSR();
    env->setAttackRate(4 * SAMPLE_RATE);
    env->setDecayRate(4 * SAMPLE_RATE);
    env->setSustainLevel(0.5);
    env->setReleaseRate(3 * SAMPLE_RATE);

    dynamic_view = 0;//TODO STRUCT WIHT ALL CUR STATES INIT HERE

    genSqr(wave[0]);//A
    genSin(wave[1]);//D
    genSaw(wave[2]);//S
    genSin(wave[3]);//R
    genSaw(wave[4]);//RE

    initFbGraphics();
    initAudio();
    initTouchscreen();
    initMidi();
    initTransformer();
    setupTimer();
}

void main_end()
{
    endTimer();
    endFbGraphics();
    endAudio();
    endMidi();
    endTransformer();
    raise(SIGTERM);//TODO SEND SHUTDOWN SIGNAL INSTEAD
}

int main()
{
    audioOutWavetable = wave[3];//DEBUG
    currentScreenWavetable = wave[5];
    currentEditWavetable = wave[0];

    main_init();
    table2Screen(wave[0]);

    while(1){}

    main_end();
    return 0;
}

