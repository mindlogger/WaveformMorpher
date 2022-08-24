#include "AudioOut.hpp"
#include "FbGraphics.hpp"
#include "Touchscreen.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"
#include "ADSR.hpp"
#include "UI.hpp"
#include "Loader.hpp"
#include "WaveOsc.hpp"
#include "GlobalPreset.hpp"

#include "GlobalDefinitions.hpp"

#include <signal.h>
#include <pthread.h>
#include <iostream>

#include "pigpio.h"

#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>

#include <time.h>
#include <stdlib.h>

void sig_term_handler(int signum)
{
    std::cout << "Terminating the program" << std::endl;
    n_shutdown_flag = false;
}

void main_init()
{
    n_shutdown_flag = true;

    WEnv = new ADSR();
    WEnv->setTargetRatioA(fGP.ADSR.A.pRatio);
    WEnv->setTargetRatioDR(fGP.ADSR.D.pRatio);
    WEnv->setAttackRate(1.1 * SAMPLE_RATE);
    WEnv->setDecayRate(1.1 * SAMPLE_RATE);
    WEnv->setLoopRate(0.0 * SAMPLE_RATE);
    WEnv->setPingPong(0);
    WEnv->setSustainLevel(0.5);
    WEnv->setReleaseRate(0.1 * SAMPLE_RATE);

    AEnv = new ADSR();
    AEnv->setTargetRatioA(fGP.ADSR.A.pRatio);
    AEnv->setTargetRatioDR(fGP.ADSR.D.pRatio);
    AEnv->setAttackRate(1.1 * SAMPLE_RATE);
    AEnv->setDecayRate(1.1 * SAMPLE_RATE);
    AEnv->setLoopRate(0.0 * SAMPLE_RATE);
    AEnv->setPingPong(0);
    AEnv->setSustainLevel(0.3);
    AEnv->setReleaseRate(0.1 * SAMPLE_RATE);

    screenstate = A;
    uiState = EditView;
    dynamic_view = 0;//TODO STRUCT WITH ALL CUR STATES INIT HERE
    fourier_flag = 0;
    fft_has_been_touched_flag = 0;
    NTables = 5;

    loopingFlag = 0;
    VCAFlag = false;
    KnobBehaviour = true;

    knobEnterHeldValue[0] = 0;
    knobState[0] = true;
    knobEnterHeldValue[1] = 0;
    knobState[1] = true;
    knobEnterHeldValue[2] = 0;
    knobState[2] = true;
    knobEnterHeldValue[3] = 0;
    knobState[3] = true;

    AttackTable = wave[0];
    DecayTable = wave[1];
    SustainStartTable = wave[2];
    SustainEndTable = wave[3];
    ReleaseTable = wave[4];

    WaveTable[0] = AttackTable;
    WaveTable[1] = DecayTable;
    WaveTable[2] = SustainStartTable;
    WaveTable[3] = SustainEndTable;
    WaveTable[4] = ReleaseTable;

    patchNameIndex = 0;
    fileSelectionIndexPatch = 0;
    fileSelectionIndexWave = 1;
    fileUserWavesCount = 0;
    browsingWindowOffset = 0;

    srand((unsigned int)time(NULL));   // Initialization, should only be called once.

    genSqr(wave[0]);//A
    genSaw(wave[1]);//D
    genSqr(wave[2]);//SS
    genSaw(wave[3]);//SE
    genSin(wave[4]);//RE

    genSil(clipboard);

    setNTables();

    setupFileLoader();

    initAudio();
    setupUI();
    signal(SIGINT, sig_term_handler);
    initFbGraphics();
    initTouchscreen();
    initMidi();
    initTransformer();
    setupTimer();
}

void main_end()
{
    endTimer();
    endAudio();
    endFbGraphics();
    endMidi();
    gpioTerminate();
    endTransformer();
    //reboot(RB_POWER_OFF);
}

int main()
{
    currentEditWavetable = wave[0];

    main_init();

    renderScreen();

    pthread_t uitxt_t;
    while(n_shutdown_flag)
    {
        pthread_create(&uitxt_t,NULL,handle_input,NULL);
        pthread_join(uitxt_t,NULL);
    }
    main_end();
    return 0;
}

