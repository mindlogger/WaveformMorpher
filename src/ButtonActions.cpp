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
#include "Loader.hpp"
#include "Blur.hpp"
#include "UIRenderer.hpp"

#include "GlobalDefinitions.hpp"

#include <pigpio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <string>
#include <vector>

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
    SW6ShiftEvent = &actionInsert;
}

void assignLoadActions()
{
    clearAllActions();

    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;

    SW4Event = &actionLoadPatch;

    SW5Event = &actionBrowseUp;

    SW6Event = &actionBrowseDown;
}

void assignStoreActions()
{
    clearAllActions();

    SW1Event = &actionCharacterClick;
    SW1ShiftEvent = &actionCharacterClick;

    SW2Event = &actionCharacterClick;
    SW2ShiftEvent = &actionCharacterClick;

    SW3Event = &actionExit;
    SW3ShiftEvent = &actionCharacterClick;

    SW4Event = &actionSavePatch;
    SW4ShiftEvent = &actionRerollCharacters;

    SW5Event = &actionCharacterClick;
    SW5ShiftEvent = &actionCharacterClick;

    SW6Event = &actionCharacterClick;
    SW6ShiftEvent = &actionCharacterClick;
}

void assignPatchSettingActions()
{
    clearAllActions();

    SW5Event = &actionToggleVCA;

    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;
}

void assignInsertActions()
{
    clearAllActions();

    SW1Event = &actionInsertWave;
    SW2Event = &actionInsertWave;
    SW4Event = &actionInsertWave;
    SW5Event = &actionInsertWave;

    SW3Event = &actionExit;
}

void assignGlobalSettingActions()
{
    clearAllActions();
    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;
}

void assignBlurActions()
{
    clearAllActions();

    SW4Event = &actionApplyBlur;

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

void actionInsertWave(uint32_t tick, uint8_t id)
{
    switch(id)
    {
        case 1:
            genSin(currentEditWavetable);
        break;
        case 2:
            genSqr(currentEditWavetable);
        break;
        case 4:
            genSaw(currentEditWavetable);
        break;
        case 5:
            genSil(currentEditWavetable);
        break;
    }
    actionExit(0,0);
}
void actionCharacterClick(uint32_t tick, uint8_t id)
{
    patchName[patchNameIndex] = fileSaveCharacters[id-1];
    patchNameIndex++;
    renderScreen();
}

void actionRerollCharacters(uint32_t tick, uint8_t id)
{
    renderScreen();
}

void actionSavePatch(uint32_t tick, uint8_t id)
{
    saveFile(string(patchName));
    patchNameIndex = 0;
    free(patchName);

    uiState = EditView;
    assignMainActions();
    renderScreen();
}

void actionToggleVCA(uint32_t tick, uint8_t id)
{
    VCA_FLAG = !VCA_FLAG;
    renderScreen();
}

void dummyAction(uint32_t tick, uint8_t id)
{
}

void actionApplyBlur(uint32_t tick, uint8_t id)
{
    for(int i = 0; i<WAVE_TABLE_SIZE; i++)
    {
        wave[screenstate][i] = currentScreenWavetable[i];
    }

    uiState = EditView;
    assignMainActions();
    renderScreen();
}

void actionLoad(uint32_t tick, uint8_t id)
{
    getFilesInDirectory();

    uiState = Load;
    assignLoadActions();
    renderScreen();
}

void actionLoadPatch(uint32_t tick, uint8_t id)
{
    loadFile(filesInDirectory[fileSelectionIndex]);

    uiState = EditView;
    assignMainActions();
    renderScreen();
}

void actionBrowseUp(uint32_t tick, uint8_t id)
{
    if(fileSelectionIndex > 0)
        fileSelectionIndex--;
    if(fileSelectionIndex - browsingWindowOffset < 3 && browsingWindowOffset != 0)
        browsingWindowOffset--;
    renderScreen();
}

void actionBrowseDown(uint32_t tick, uint8_t id)
{
    if(filesInDirectory.size() -1 > fileSelectionIndex)
        fileSelectionIndex++;
    if(fileSelectionIndex - browsingWindowOffset > 6 && browsingWindowOffset + 9 < filesInDirectory.size())
        browsingWindowOffset++;
    renderScreen();
}

void actionStore(uint32_t tick, uint8_t id)
{
    uiState = Store;
    assignStoreActions();
    renderScreen();
}

void actionWaveStep(uint32_t tick, uint8_t id)
{
    screenstate = (Screenstates) (((int)screenstate + 1) % NTables);
    if(fourier_flag)
    {
        currentEditWavetable = fft[screenstate];
    }
    else
    {
        currentEditWavetable = wave[screenstate];
    }
    renderScreen();
}

void actionWaveN(uint32_t tick, uint8_t id) //TODO CHANGE THIS TO THE CORRECT FUNCTION
{
    NTables = NTables % 5;
    NTables += 1;
    setNTables();
    renderScreen();
    addText(std::to_string(NTables),2,80,1);
}

void actionOpenPatchSettings(uint32_t tick, uint8_t id)
{
    uiState = PatchSettings;
    renderScreen();
    assignPatchSettingActions();
}

void actionOpenGlobalSettings(uint32_t tick, uint8_t id)
{
    uiState = GlobalSettings;
    renderScreen();
    assignGlobalSettingActions();
}

void actionExit(uint32_t tick, uint8_t id)
{
    switch(uiState)
    {
        case EditView:
        break;
        case DynamicView:
        break;
        case PatchSettings:
        break;
        case GlobalSettings:
        break;
        case Load:
        break;
        case Store:
            patchNameIndex = 0;
            free(patchName);
        break;
        case InsertWave:
        break;
        case HiddenMode:
        break;
    }

    uiState = EditView;
    renderScreen();
    assignMainActions();
}

void actionQuestion(uint32_t tick, uint8_t id)
{
    if(dynamic_view)
        dynamic_view = 0;
    else
        dynamic_view = 1;
    renderScreen();
}

void actionQuestionS(uint32_t tick, uint8_t id)
{
    if(!fourier_flag)
    {
        uiState = BlurMode;
        assignBlurActions();
    }
    else
    {
        addText("Exit FFT for Blur Mode", calcCenterOfText("Exit FFT for Blur Mode"), SCREEN_HEADER_Y, 2);
    }
}

void actionCopy(uint32_t tick, uint8_t id)
{
    memcpy(&clipboard,currentEditWavetable,WAVE_TABLE_SIZE * sizeof(double));
    renderScreen();
    clearScreen();
    insertCurrentTableName();
    addText("Copy", calcCenterOfText("Copy"), SCREEN_HEADER_Y, 1);
}

void actionPaste(uint32_t tick, uint8_t id)
{
    memcpy(currentEditWavetable,&clipboard,WAVE_TABLE_SIZE * sizeof(double));
    renderScreen();
    clearScreen();
    insertCurrentTableName();
    addText("Paste", calcCenterOfText("Paste"), SCREEN_HEADER_Y, 1);
}

void actionFourier(uint32_t tick, uint8_t id)
{
    if(fourier_flag)
    {
        currentEditWavetable = wave[screenstate];
        transBackward(screenstate);
        fourier_flag = 0;
        renderScreen();
        clearScreen();
        insertCurrentTableName();
        addText("IFFT", calcCenterOfText("IFFT"), SCREEN_HEADER_Y, 1);
    }
    else
    {
        currentEditWavetable = fft[screenstate];
        transForward(screenstate);
        fourier_flag = 1;
        renderScreen();
        clearScreen();
        insertCurrentTableName();
        addText("FFT", calcCenterOfText("FFT"), SCREEN_HEADER_Y, 1);
    }
}

void actionInsert(uint32_t tick, uint8_t id)
{
    uiState = InsertWave;
    renderScreen();
    assignInsertActions();
}
