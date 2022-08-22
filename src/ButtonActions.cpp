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
#include "GlobalPreset.hpp"

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
    SW1Event = &actionBrowsePatch;
    SW1ShiftEvent = &actionStorePatch;

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

void assignLoadPatchActions()
{
    clearAllActions();

    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;

    SW4Event = &actionLoadPatch;

    SW5Event = &actionPatchBrowseUp;

    SW6Event = &actionPatchBrowseDown;
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
    SW6Event = &actionToggleKnobBehaviourPatch;

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
    SW6Event = &actionBrowseWave;
    SW3Event = &actionExit;
}

void assignInsertUserActions()
{
    clearAllActions();

    SW2Event = &actionStoreWave;

    SW2ShiftEvent = &actionDeleteWave;

    SW3Event = &actionExit;

    SW4Event = &actionLoadWave;

    SW5Event = &actionWaveBrowseUp;

    SW6Event = &actionWaveBrowseDown;
}

void assignGlobalSettingActions()
{
    clearAllActions();

    SW5Event = &actionToggleContinous;
    SW6Event = &actionToggleKnobBehaviourGlobal;

    SW3Event = &actionExit;
    SW3ShiftEvent = &actionExit;
}

void assignBlurActions()
{
    clearAllActions();

    SW4Event = &actionApplyBlur;

    SW3Event = &actionUndoBlur;
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

void actionToggleKnobBehaviourPatch(uint32_t tick, uint8_t id)
{
    if(fGP.KnobResponse == PerPatch)
    {
        KnobBehaviour = ! KnobBehaviour;
        renderScreen();
    }
}

void actionToggleKnobBehaviourGlobal(uint32_t tick, uint8_t id)
{
    fGP.KnobResponse = (EKnobResponseTypes) ( (fGP.KnobResponse + 1) % 3);
    renderScreen();
}

void actionToggleContinous(uint32_t tick, uint8_t id)
{
    fGP.Visual.continous = ! fGP.Visual.continous;
    renderScreen();
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
    savePatchToFile(string(patchName));
    patchNameIndex = 0;
    free(patchName);

    uiState = EditView;
    assignMainActions();
    renderScreen();
}

void actionToggleVCA(uint32_t tick, uint8_t id)
{
    VCAFlag = !VCAFlag;
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

    actionExit(0,0);
}

void actionUndoBlur(uint32_t tick, uint8_t id)
{
    for(int i = 0; i<WAVE_TABLE_SIZE; i++)
    {
        wave[screenstate][i] = fftEnterSnapshot[i];
    }

    actionExit(0,0);
}

void actionBrowsePatch(uint32_t tick, uint8_t id)
{
    getFilesInDirectory();

    uiState = Load;
    assignLoadPatchActions();
    renderScreen();
}

void actionLoadPatch(uint32_t tick, uint8_t id)
{
    loadPatchFromFile(filesInDirectory[fileSelectionIndexPatch]);

    patchNameIndex = 0;
    fileSelectionIndexPatch = 0;
    browsingWindowOffset = 0;

    actionExit(0,0);
}

void actionPatchBrowseUp(uint32_t tick, uint8_t id)
{
    if(fileSelectionIndexPatch > 0)
        fileSelectionIndexPatch--;
    if(fileSelectionIndexPatch - browsingWindowOffset < 3 && browsingWindowOffset != 0)
        browsingWindowOffset--;
    renderScreen();
}

void actionPatchBrowseDown(uint32_t tick, uint8_t id)
{
    if(filesInDirectory.size() -1 > fileSelectionIndexPatch)
        fileSelectionIndexPatch++;
    if(fileSelectionIndexPatch - browsingWindowOffset > 6 && browsingWindowOffset + 9 < filesInDirectory.size())
        browsingWindowOffset++;
    renderScreen();
}

void actionStorePatch(uint32_t tick, uint8_t id)
{
    uiState = Store;
    assignStoreActions();
    renderScreen();
}

void actionStoreWave(uint32_t tick, uint8_t id)
{
    saveWaveToFile();
    updateUserWavesCount();

    fileSelectionIndexWave = 1;

    actionExit(0,0);
}

void actionBrowseWave(uint32_t tick, uint8_t id)
{
    loadWaveFromFile(fileSelectionIndexWave);
    uiState = UserInsertWave;
    assignInsertUserActions();
    renderScreen();
}

void actionLoadWave(uint32_t tick, uint8_t id)
{
    for(int i = 0; i < WAVE_TABLE_SIZE; i++)
    {
        wave[screenstate][i] = currentScreenWavetable[i];
    }

    fileSelectionIndexWave = 1;

    actionExit(0,0);
}

void actionDeleteWave(uint32_t tick, uint8_t id)
{
    //deleteWaveFile(fileSelectionIndexWave); //IMPLEMENT THIS IN THE FUTURE WITH PROPER FILE BROWSING/HANDLING
}

void actionWaveBrowseUp(uint32_t tick, uint8_t id)
{
    if(fileSelectionIndexWave > 1)
    {
        fileSelectionIndexWave--;
        loadWaveFromFile(fileSelectionIndexWave);
    }
    renderScreen();
}

void actionWaveBrowseDown(uint32_t tick, uint8_t id)
{
    if(fileUserWavesCount > fileSelectionIndexWave)
    {
        fileSelectionIndexWave++;
        loadWaveFromFile(fileSelectionIndexWave);
    }
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
        case BlurMode:
            setNTables();
        break;
        case UserInsertWave:
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
        for(int i = 0; i < WAVE_TABLE_SIZE; i++) //APPLY SNAPSHOT FOR UNDOING
        {
            fftEnterSnapshot[i] = wave[screenstate][i];
        }
        setCueTable();
        uiState = BlurMode;
        assignBlurActions();
    }
    else
    {
        for(int i = 0; i < WAVE_TABLE_SIZE; i++) //APPLY SNAPSHOT FOR UNDOING
        {
            fft[screenstate][i] = fftEnterSnapshot[i];
        }
        fft_has_been_touched_flag = 1;

        renderScreen();
        clearScreen();
        insertCurrentTableName();
        addText("Undo", calcCenterOfText("Undo"), SCREEN_HEADER_Y, 1);
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

        for(int i = 0; i < WAVE_TABLE_SIZE; i++) //GET A SNAPSHOT FOR UNDOING
        {
            fftEnterSnapshot[i] = fft[screenstate][i];
        }

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
