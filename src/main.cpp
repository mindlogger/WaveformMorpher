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
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>

void main_init()
{
    pthread_t render_t;
    pthread_create(&render_t,NULL,RenderScreen,NULL);

    env = new ADSR();
    env->setAttackRate(4 * SAMPLE_RATE);
    env->setDecayRate(4 * SAMPLE_RATE);
    env->setSustainLevel(0.3);
    env->setReleaseRate(4 * SAMPLE_RATE);

    dynamic_view = 1;//TODO STRUCT WIHT ALL CUR STATES INIT HERE

    genSqr(wave[0]);//A
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
    audioOutWavetable = wave[3];//DEBUG
    currentEditWavetable = wave[0];

    main_init();

    pthread_t ui_t;
    while(1)
    {
        pthread_create(&ui_t,NULL,handle_input,NULL);
        pthread_join(ui_t,NULL);
    }

    main_end();
    return 0;
}

