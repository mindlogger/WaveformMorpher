#include "FbGraphics.hpp"
#include "GlobalDefinitions.hpp"
#include "GlobalPreset.hpp"
#include "Blur.hpp"

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

using namespace std;

extern "C"
{
#include "fbgraphics.h"
}

void insertCurrentTableName()
{
    if(dynamic_view)
    {
        switch(screenstate)
        {
        case A:
            fbg_text(fbg, bbfont, "1 - A", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
        break;
        case D:
            switch(NTables)
            {
                case 1:
                    fbg_text(fbg, bbfont, "1 - D", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 2:
                    fbg_text(fbg, bbfont, "2 - R", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 3:
                    fbg_text(fbg, bbfont, "2 - D", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 4:
                    fbg_text(fbg, bbfont, "2 - D", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 5:
                    fbg_text(fbg, bbfont, "2 - D", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
            }
        break;
        case SS:
            switch(NTables)
            {
                case 1:
                    fbg_text(fbg, bbfont, "1 - SS", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 2:
                    fbg_text(fbg, bbfont, "1 - SS", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 3:
                    fbg_text(fbg, bbfont, "3 - R", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 4:
                    fbg_text(fbg, bbfont, "3 - SS", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 5:
                    fbg_text(fbg, bbfont, "3 - SS", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
            }
        break;
        case SE:
            switch(NTables)
            {
                case 1:
                    fbg_text(fbg, bbfont, "1 - SE", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 2:
                    fbg_text(fbg, bbfont, "1 - SE", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 3:
                    fbg_text(fbg, bbfont, "2 - SE", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 4:
                    fbg_text(fbg, bbfont, "4 - R", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 5:
                    fbg_text(fbg, bbfont, "3 - SE", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
            }
        break;
        case R:
            switch(NTables)
            {
                case 1:
                    fbg_text(fbg, bbfont, "1 - R", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 2:
                    fbg_text(fbg, bbfont, "2 - R", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 3:
                    fbg_text(fbg, bbfont, "3 - R", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 4:
                    fbg_text(fbg, bbfont, "4 - R", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
                case 5:
                    fbg_text(fbg, bbfont, "5 - R", 4, 2, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
                break;
            }
        break;
        }
    }
    else
    {
        switch(screenstate)
        {
            case A:
                fbg_write(fbg, "1 - A", 4, 2);
            break;
            case D:
                switch(NTables)
                {
                    case 1:
                        fbg_write(fbg, "1 - D", 4, 2);
                    break;
                    case 2:
                        fbg_write(fbg, "2 - R", 4, 2);
                    break;
                    case 3:
                        fbg_write(fbg, "2 - D", 4, 2);
                    break;
                    case 4:
                        fbg_write(fbg, "2 - D", 4, 2);
                    break;
                    case 5:
                        fbg_write(fbg, "2 - D", 4, 2);
                    break;
                }
            break;
            case SS:
                switch(NTables)
                {
                    case 1:
                        fbg_write(fbg, "1 - SS", 4, 2);
                    break;
                    case 2:
                        fbg_write(fbg, "1 - SS", 4, 2);
                    break;
                    case 3:
                        fbg_write(fbg, "3 - R", 4, 2);
                    break;
                    case 4:
                        fbg_write(fbg, "3 - SS", 4, 2);
                    break;
                    case 5:
                        fbg_write(fbg, "3 - SS", 4, 2);
                    break;
                }
            break;
            case SE:
                switch(NTables)
                {
                    case 1:
                        fbg_write(fbg, "1 - SE", 4, 2);
                    break;
                    case 2:
                        fbg_write(fbg, "1 - SE", 4, 2);
                    break;
                    case 3:
                        fbg_write(fbg, "2 - SE", 4, 2);
                    break;
                    case 4:
                        fbg_write(fbg, "4 - R", 4, 2);
                    break;
                    case 5:
                        fbg_write(fbg, "3 - SE", 4, 2);
                    break;
                }
            break;
            case R:
                switch(NTables)
                {
                    case 1:
                        fbg_write(fbg, "1 - R", 4, 2);
                    break;
                    case 2:
                        fbg_write(fbg, "2 - R", 4, 2);
                    break;
                    case 3:
                        fbg_write(fbg, "3 - R", 4, 2);
                    break;
                    case 4:
                        fbg_write(fbg, "4 - R", 4, 2);
                    break;
                    case 5:
                        fbg_write(fbg, "5 - R", 4, 2);
                    break;
                }
            break;
        }
    }
}

void renderEditView()
{
    clearScreen();
    table2Screen(currentEditWavetable);

    clearScreen();
    table2Screen(currentEditWavetable);

    if(!knobState[0])
        fbg_text(fbg, bbfont, &(std::to_string( (int) (knobEnterHeldValue[0]/40.690)) + "%")[0], SCREEN_SW1_3_POSX + 16 - ( (uint32_t) (log10( (uint32_t) (knobEnterHeldValue[0]/40.690) ) ) )*8, SCREEN_SW3_6_POSY, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
    if(!knobState[1])
        fbg_text(fbg, bbfont, &(std::to_string( (int) (knobEnterHeldValue[1]/40.690)) + "%")[0], SCREEN_SW1_3_POSX + 177 - ( (uint32_t) (log10( (uint32_t) (knobEnterHeldValue[1]/40.690) ) ) )*8, SCREEN_SW3_6_POSY, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
    if(!knobState[2])
        fbg_text(fbg, bbfont, &(std::to_string( (int) (knobEnterHeldValue[2]/40.690)) + "%")[0], SCREEN_SW1_3_POSX + 328 - ( (uint32_t) (log10( (uint32_t) (knobEnterHeldValue[2]/40.690) ) ) )*8, SCREEN_SW3_6_POSY, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
    if(!knobState[3])
        fbg_text(fbg, bbfont, &(std::to_string( (int) (knobEnterHeldValue[3]/40.690)) + "%")[0], SCREEN_SW4_6_POSX - 16 - ( (uint32_t) (log10( (uint32_t) (knobEnterHeldValue[3]/40.690) ) ) )*16, SCREEN_SW3_6_POSY, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);

    insertCurrentTableName();
    commitScreenBuffer();
}

void renderPatchSettings()
{
    clearScreen();

    fbg_write(fbg, "Patch Settings", calcCenterOfText("Patch Settings"), SCREEN_HEADER_Y);

    if(VCAFlag)
        fbg_write(fbg, "VCA:On", SCREEN_SW4_6_POSX - 5*16, SCREEN_SW2_5_POSY);
    else
        fbg_write(fbg, "VCA:Off", SCREEN_SW4_6_POSX - 6*16, SCREEN_SW2_5_POSY);
    if(fGP.UI.KnobResponse == PerPatch)
    {
        if(KnobBehaviour)
            fbg_write(fbg, "KnobB:EditViewOnly", SCREEN_SW4_6_POSX - 17*16, SCREEN_SW3_6_POSY);
        else
            fbg_write(fbg, "KnobB:Always", SCREEN_SW4_6_POSX - 11*16, SCREEN_SW3_6_POSY);
    }
    else
    {
            fbg_write(fbg, "KnobB:GlobalSettings", SCREEN_SW4_6_POSX - 19*16, SCREEN_SW3_6_POSY);
    }

    fbg_text(fbg, bbfont , "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r);
    commitScreenBuffer();
}

void renderGlobalSettings()
{
    clearScreen();

    fbg_write(fbg, "Global Settings", calcCenterOfText("Global Settings"), SCREEN_HEADER_Y);

    if(fGP.Visual.continous)
        fbg_write(fbg, "Wave:Continous", SCREEN_SW4_6_POSX - 13*16, SCREEN_SW2_5_POSY);
    else
        fbg_write(fbg, "Wave:Discrete", SCREEN_SW4_6_POSX - 12*16, SCREEN_SW2_5_POSY);

    switch(fGP.UI.KnobResponse)
    {
        case EditViewOnly:
            fbg_write(fbg, "KnobB:EditViewOnly", SCREEN_SW4_6_POSX - 17*16, SCREEN_SW3_6_POSY);
        break;
        case PerPatch:
            fbg_write(fbg, "KnobB:PerPatch", SCREEN_SW4_6_POSX - 13*16, SCREEN_SW3_6_POSY);
        break;
        case Always:
            fbg_write(fbg, "KnobB:Always", SCREEN_SW4_6_POSX - 11*16, SCREEN_SW3_6_POSY);
        break;
    }

    fbg_text(fbg, bbfont , "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r);
    commitScreenBuffer();
}

void renderLoad()
{
    clearScreen();

    for(int i = 0; i < 9 && i < filesInDirectory.size(); i++)
    {
        if(fileSelectionIndexPatch == i + browsingWindowOffset)
        {
            fbg_text(fbg, bbfont, "o", calcCenterOfText(filesInDirectory[i + browsingWindowOffset]) - 2*16, 48 + (i*30), fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
            fbg_text(fbg, bbfont, "o", calcCenterOfText(filesInDirectory[i + browsingWindowOffset]) + (filesInDirectory[i + browsingWindowOffset].length() * 16) + 16, 49 + (i*30), fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
        }
        fbg_write(fbg, &filesInDirectory[i + browsingWindowOffset][0], calcCenterOfText(filesInDirectory[i + browsingWindowOffset]), 50 + (i*30));
    }
    fbg_text(fbg, bbfont , "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r);
    fbg_text(fbg, bbfont , ">", SCREEN_SW4_6_POSX, SCREEN_SW1_4_POSY, fGP.Color.Confirm.b, fGP.Color.Confirm.g, fGP.Color.Confirm.r);
    fbg_write(fbg, "Load Patch", calcCenterOfText("Load Patch"), SCREEN_HEADER_Y);
    commitScreenBuffer();
}

void renderBlurMode()
{
    clearScreen();

    fbg_text(fbg, bbfont , "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r);
    fbg_text(fbg, bbfont , ">", SCREEN_SW4_6_POSX, SCREEN_SW1_4_POSY, fGP.Color.Confirm.b, fGP.Color.Confirm.g, fGP.Color.Confirm.r);

    uint8_t algorithm = (uint8_t) ( (knob4Value - 4095) * (1 - 4) / (0 - 4095) + 4);

    switch(algorithm)
    {
        case 1:
        {
            double threshold = (knob1Value - 4095) * (fGP.KBlur.thrsMax - fGP.KBlur.thrsMin) / (0 - 4095) + fGP.KBlur.thrsMin;
            double gain = (knob2Value - 4095) * (fGP.KBlur.gainMax - fGP.KBlur.gainMin) / (0 - 4095) + fGP.KBlur.gainMin;
            uint16_t window = (knob3Value - 4095) * (fGP.KBlur.windowMax - fGP.KBlur.windowMin) / (0 - 4095) + fGP.KBlur.windowMin;
            applyKBlurForward(wave[screenstate], currentScreenWavetable, threshold , gain, window);
            fbg_write(fbg, "K-Blur-F", calcCenterOfText("K-Blur-F"), SCREEN_HEADER_Y);
        }
        break;
        case 2:
        {
            double threshold = (knob1Value - 4095) * (fGP.KBlur.thrsMax - fGP.KBlur.thrsMin) / (0 - 4095) + fGP.KBlur.thrsMin;
            double gain = (knob2Value - 4095) * (fGP.KBlur.gainMax - fGP.KBlur.gainMin) / (0 - 4095) + fGP.KBlur.gainMin;
            uint16_t window = (knob3Value - 4095) * (fGP.KBlur.windowMax - fGP.KBlur.windowMin) / (0 - 4095) + fGP.KBlur.windowMin;
            applyKBlurBackward(wave[screenstate], currentScreenWavetable, threshold , gain, window);
            fbg_write(fbg, "K-Blur-B", calcCenterOfText("K-Blur-B"), SCREEN_HEADER_Y);
        }
        break;
        case 3:
        {
            uint16_t rounds = (knob1Value - 4095) * (fGP.MBlur.roundsMin - fGP.MBlur.roundsMax) / (0 - 4095) + fGP.MBlur.roundsMax;
            applyMedianBlur(wave[screenstate], currentScreenWavetable, rounds);
            fbg_write(fbg, "M-Blur", calcCenterOfText("M-Blur"), SCREEN_HEADER_Y);
        }
        break;
        default:
        {
            uint16_t rounds = (knob1Value - 4095) * (fGP.MBlur.roundsMin - fGP.MBlur.roundsMax) / (0 - 4095) + fGP.MBlur.roundsMax;
            applyMedianBlur(wave[screenstate], currentScreenWavetable, rounds);
            fbg_write(fbg, "M-Blur", calcCenterOfText("M-Blur"), SCREEN_HEADER_Y);
        }
        break;
    }

    table2Screen(currentScreenWavetable);
}

#define MAX_PATCH_NAME_LENGTH 16
char characterUpercaseOptions[39] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_-"; //ACTUALLY ONLY 38 CHARACTERS
char characterOptions[39] = "abcdefghijklmnopqrstuvwxyz1234567890_-"; //ACTUALLY ONLY 38 CHARACTERS

char SW1Text[3];
char SW2Text[3];
char SW3Text[3];
char SW4Text[3];
char SW5Text[3];
char SW6Text[3];

void renderStore()
{
    clearScreen();

    if(patchNameIndex == 0)
    {
        patchName = (char*)malloc(MAX_PATCH_NAME_LENGTH);
        memset(&patchName[0], 0, MAX_PATCH_NAME_LENGTH);

        //DO INIT SHIT
        for(int i = 0; i < 12; i++)
        {
            fileSaveCharacters[i] = characterUpercaseOptions[rand() % 39];
        }
    }
    else
    {
        fbg_write(fbg, patchName, calcCenterOfText(patchName), SCREEN_HEADER_Y + SCREEN_SECOND_HEADER_Y);

        for(int i = 0; i < 12; i++)
        {
            fileSaveCharacters[i] = characterOptions[rand() % 39];
        }
    }

    //FIND THE RANDOMLY ASSIGNED LETTERS STORE + DISPLAY THEM

    fbg_write(fbg, "Store Patch", calcCenterOfText("Store Patch"), SCREEN_HEADER_Y);

    SW1Text[0] = fileSaveCharacters[0];
    SW1Text[1] = '/';
    SW1Text[2] = fileSaveCharacters[6];

    SW2Text[0] = fileSaveCharacters[1];
    SW2Text[1] = '/';
    SW2Text[2] = fileSaveCharacters[7];

    SW3Text[0] = ' ';
    SW3Text[1] = '/';
    SW3Text[2] = fileSaveCharacters[8];

    SW4Text[0] = ' ';
    SW4Text[1] = '/';
    SW4Text[2] = ' ';

    SW5Text[0] = fileSaveCharacters[4];
    SW5Text[1] = '/';
    SW5Text[2] = fileSaveCharacters[10];

    SW6Text[0] = fileSaveCharacters[5];
    SW6Text[1] = '/';
    SW6Text[2] = fileSaveCharacters[11];

    fbg_write(fbg, SW1Text, SCREEN_SW1_3_POSX, SCREEN_SW1_4_POSY);
    fbg_write(fbg, SW2Text, SCREEN_SW1_3_POSX, SCREEN_SW2_5_POSY);

    fbg_text(fbg, bbfont , "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r);
    fbg_text(fbg, bbfont , "R", SCREEN_SW4_6_POSX, SCREEN_SW1_4_POSY, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
    fbg_text(fbg, bbfont , ">", SCREEN_SW4_6_POSX - 16*2, SCREEN_SW1_4_POSY, fGP.Color.Confirm.b, fGP.Color.Confirm.g, fGP.Color.Confirm.r);

    fbg_write(fbg, SW3Text, SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    fbg_write(fbg, SW4Text, SCREEN_SW4_6_POSX - 33, SCREEN_SW1_4_POSY);
    fbg_write(fbg, SW5Text, SCREEN_SW4_6_POSX - 33, SCREEN_SW2_5_POSY);
    fbg_write(fbg, SW6Text, SCREEN_SW4_6_POSX - 33, SCREEN_SW3_6_POSY);

    commitScreenBuffer();
}

void renderInsertWave()
{
    clearScreen();

    fbg_write(fbg, "Insert Wave", calcCenterOfText("Insert Wave"), SCREEN_HEADER_Y);

    fbg_write(fbg, "Sine", SCREEN_SW1_3_POSX, SCREEN_SW1_4_POSY);
    fbg_write(fbg, "Square", SCREEN_SW1_3_POSX, SCREEN_SW2_5_POSY);
    fbg_write(fbg, "Saw", SCREEN_SW4_6_POSX - 2*16, SCREEN_SW1_4_POSY);
    fbg_write(fbg, "Silence", SCREEN_SW4_6_POSX - 6*16, SCREEN_SW2_5_POSY);
    fbg_write(fbg, "User", SCREEN_SW4_6_POSX - 3*16, SCREEN_SW3_6_POSY);
    fbg_write(fbg, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);

    commitScreenBuffer();
}

void renderUserInsertWave()
{
    clearScreen();

    table2Screen(currentScreenWavetable);

    clearScreen();

    table2Screen(currentScreenWavetable);

    fbg_write(fbg, "User Wave", calcCenterOfText("User Wave"), SCREEN_HEADER_Y);
    fbg_write(fbg, &(std::to_string(fileSelectionIndexWave))[0] , calcCenterOfText(std::to_string(fileSelectionIndexWave)), SCREEN_SECOND_HEADER_Y);

    /*SW2Text[0] = ' ';
    SW2Text[1] = '/';
    SW2Text[2] = ' ';*/

    //fbg_write(fbg, SW2Text, SCREEN_SW1_3_POSX, SCREEN_SW2_5_POSY);
    fbg_text(fbg, bbfont, "W", SCREEN_SW1_3_POSX, SCREEN_SW2_5_POSY, fGP.Color.HighlightA.b, fGP.Color.HighlightA.g, fGP.Color.HighlightA.r);
    //fbg_text(fbg, bbfont , "x", SCREEN_SW1_3_POSX + 2*16, SCREEN_SW2_5_POSY - 1, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r); //IMPLEMENT THIS IN THE FUTURE WITH PROPER FILE BROWSING/HANDLING

    fbg_text(fbg, bbfont, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r);
    fbg_text(fbg, bbfont, ">", SCREEN_SW4_6_POSX, SCREEN_SW1_4_POSY, fGP.Color.Confirm.b, fGP.Color.Confirm.g, fGP.Color.Confirm.r);

    commitScreenBuffer();
}

void renderHiddenMode()
{}
