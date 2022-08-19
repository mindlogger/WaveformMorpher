#include "AudioOut.hpp"
#include "FbGraphics.hpp"
#include "Touchscreen.hpp"
#include "WaveOsc.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"
#include "FourierTransformer.hpp"
#include "ADSR.hpp"
#include "UI.hpp"
#include "ButtonActions.hpp"

#include "GlobalDefinitions.hpp"

#include <pigpio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

void assignMainActions()
{
    SW1Event = &actionLoad;
    SW1ShiftEvent = &actionStore;

    SW2Event = &actionWaveStep;
    SW2ShiftEvent = &actionWaveN;

    SW3Event = &actionOpenPatchSettings;
    SW3ShiftEvent = &actionOpenGlobalSettings;

    SW4Event = &actionQuestion;
    SW4ShiftEvent = &actionQuestionS;

    SW5Event = &actionCopy;
    SW5ShiftEvent = &actionPaste;

    SW6Event = &actionFourier;
    SW6ShiftEvent = &actionInverse;
}

void assignLoadActions()
{
    clearAllActions();
    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;
}

void assignStoreActions()
{
    clearAllActions();
    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;
}

void assignPatchSettingActions()
{
    clearAllActions();
    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;
}

void assignGlobalSettingActions()
{
    clearAllActions();
    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;
}

void clearAllActions()
{
    SW1Event = &dummyAction;
    SW1ShiftEvent = &dummyAction;

    SW2Event = &dummyAction;
    SW2ShiftEvent = &dummyAction;

    SW3Event = &dummyAction;
    SW3ShiftEvent = &dummyAction;

    SW4Event = &dummyAction;
    SW4ShiftEvent = &dummyAction;

    SW5Event = &dummyAction;
    SW5ShiftEvent = &dummyAction;

    SW6Event = &dummyAction;
    SW6ShiftEvent = &dummyAction;
}

void dummyAction(uint32_t tick)
{
}

void actionLoad(uint32_t tick)
{
    uiState = Load;
    renderScreen();
    assignLoadActions();
}
void actionStore(uint32_t tick)
{
    uiState = Store;
    renderScreen();
    assignStoreActions();
}

int preset_wave_step = 0; //TOIDO MOVE THIS SOMEWHERE MORE SENSIBLE
void actionWaveStep(uint32_t tick)
{
    screenstate = (Screenstates) (((int)screenstate + 1) % 5);
    if(fourier_flag)
    {
    currentEditWavetable = fft[screenstate];
    cout << screenstate + 1 << " Spectrum" << endl;
    }
    else
    {
    currentEditWavetable = wave[screenstate];
    cout << screenstate + 1 << " Wave" << endl;
    }
    renderScreen();
}
void actionWaveN(uint32_t tick) //TODO CHANGE THIS TO THE CORRECT FUNCTION
{
    NTables += 1;
    NTables = NTables % 6;
    addText(std::to_string(NTables),2,80,1);
}

void actionOpenPatchSettings(uint32_t tick)
{
    uiState = PatchSettings;
    renderScreen();
    assignPatchSettingActions();
}
void actionOpenGlobalSettings(uint32_t tick)
{
    uiState = GlobalSettings;
    renderScreen();
    assignGlobalSettingActions();
}
void actionExit(uint32_t tick)
{
    uiState = EditView;
    renderScreen();
    assignMainActions();
}

void actionQuestion(uint32_t tick)
{
    std::cout << "Question got triggerd" << std::endl;
    dynamic_view = 1;
}
void actionQuestionS(uint32_t tick)
{
    std::cout << "QuestionS got triggerd" << std::endl;
    renderScreen();
    dynamic_view = 0;

}

void actionCopy(uint32_t tick)
{
    std::cout << "WAVE COPY" << std::endl;
    memcpy(&clipboard,currentEditWavetable,WAVE_TABLE_SIZE * sizeof(double));
}
void actionPaste(uint32_t tick)
{
    std::cout << "WAVE PASTE" << std::endl;
    memcpy(currentEditWavetable,&clipboard,WAVE_TABLE_SIZE * sizeof(double));
    renderScreen();
}

void actionFourier(uint32_t tick)
{
    if(fourier_flag)
    {
        cout << "backwards transform" << endl;
        currentEditWavetable = wave[screenstate];
        transBackward(screenstate);
        fourier_flag = 0;
        renderScreen();
        addText("W", 460, 260,1);
    }
    else
    {
        cout << "forward transform" << endl;
        currentEditWavetable = fft[screenstate];
        transForward(screenstate);
        fourier_flag = 1;
        renderScreen();
        addText("S", 460, 260,1);
    }
}
void actionInverse(uint32_t tick)
{
    preset_wave_step = (preset_wave_step + 1) % 4;
    std::cout << preset_wave_step << std::endl;
    switch(preset_wave_step)
    {
    case 0:
        genSil(currentEditWavetable);
    break;
    case 1:
        genSqr(currentEditWavetable);
    break;
    case 2:
        genSaw(currentEditWavetable);
    break;
    case 3:
        genSin(currentEditWavetable);
    break;
    }
    renderScreen();
}
