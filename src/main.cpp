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

void main_init()
{
    sus_v = 0.3;
    envelope = new ADSR();
    setTargetRatioA(0.3);
    setTargetRatioDR(0.3);
    envelope->setAttackRate(4 * SAMPLE_RATE);
    envelope->setDecayRate(4 * SAMPLE_RATE);
    envelope->setSustainLevel(sus_v);
    envelope->setReleaseRate(4 * SAMPLE_RATE);

    screenstate = A;
    dynamic_view = 0;//TODO STRUCT WIHT ALL CUR STATES INIT HERE
    fourier_flag = 0;
    fft_has_been_touched_flag = 0;

    genSin(wave[0]);//A
    genSaw(wave[1]);//D
    genSqr(wave[2]);//S
    genSin(wave[3]);//R
    genSil(wave[4]);//RE

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
    currentEditWavetable = wave[0];

    main_init();

    table2Screen(currentEditWavetable);

    pthread_t ui_t;
    while(1)//TODO WHILE NOT SHUTDOWN
    {
        pthread_create(&ui_t,NULL,handle_input,NULL);
        pthread_join(ui_t,NULL);
    }
    main_end();
    return 0;
}

