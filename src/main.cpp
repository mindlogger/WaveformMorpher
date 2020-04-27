#include "AudioOut.hpp"
#include "FbGraphics.hpp"
#include "Touchscreen.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"
#include "ADSR.hpp"
#include "UI.hpp"

#include "GlobalDefinitions.hpp"

#include <signal.h>
#include <pthread.h>
#include <iostream>

#include "pigpio.h"

void main_init()
{
    n_shutdown_flag = 1;
    sus_v = 0.3;
    envelope = new ADSR();
    envelope->setTargetRatioA(0.3);
    envelope->setTargetRatioDR(0.3);
    envelope->setAttackRate(0.1 * SAMPLE_RATE);
    envelope->setDecayRate(0.1 * SAMPLE_RATE);
    envelope->setLoopRate(2 * SAMPLE_RATE);
    envelope->setPingPong(1);
    envelope->setSustainLevel(sus_v);
    envelope->setReleaseRate(0.1 * SAMPLE_RATE);

    screenstate = A;
    dynamic_view = 0;//TODO STRUCT WIHT ALL CUR STATES INIT HERE
    fourier_flag = 0;
    fft_has_been_touched_flag = 0;

    genSqr(wave[0]);//A
    genSaw(wave[1]);//D
    genSqr(wave[2]);//SS
    genSaw(wave[3]);//SE
    genSin(wave[4]);//RE

    initFbGraphics();
    initAudio();
    setupUI();
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
    gpioTerminate();
    raise(SIGTERM);//TODO SEND SHUTDOWN SIGNAL INSTEAD
}
void sig_term_handler(int signum)//DEBUGING PURPOSES
{
    gpioTerminate();
    n_shutdown_flag = 0;
}
int main()
{
    signal(SIGINT, sig_term_handler);

    currentEditWavetable = wave[0];

    main_init();

    table2Screen(currentEditWavetable);

    pthread_t uitxt_t;
    while(n_shutdown_flag)//TODO WHILE NOT SHUTDOWN
    {
        pthread_create(&uitxt_t,NULL,handle_input,NULL);
        pthread_join(uitxt_t,NULL);
    }
    main_end();
    return 0;
}

