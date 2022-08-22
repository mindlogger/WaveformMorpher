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
    insertCurrentTableName();
    table2Screen(currentEditWavetable);
}

void renderPatchSettings()
{
    clearScreen();

    fbg_write(fbg, "Patch Settings", calcCenterOfText("Patch Settings"), SCREEN_HEADER_Y);
    if(VCA_FLAG)
        fbg_write(fbg, "VCA:On", SCREEN_SW4_6_POSX - 100, SCREEN_SW2_5_POSY);
    else
        fbg_write(fbg, "VCA:Off", SCREEN_SW4_6_POSX - 100, SCREEN_SW2_5_POSY);

    fbg_text(fbg, bbfont , "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r);
    commitScreenBuffer();
}

void renderGlobalSettings()
{
    clearScreen();

    fbg_write(fbg, "Global Settings", calcCenterOfText("Global Settings"), SCREEN_HEADER_Y);
    fbg_text(fbg, bbfont , "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY, fGP.Color.Cancel.b, fGP.Color.Cancel.g, fGP.Color.Cancel.r);
    commitScreenBuffer();
}

void renderLoad()
{
    clearScreen();

    for(int i = 0; i < 9 && i < filesInDirectory.size(); i++)
    {
        if(fileSelectionIndex == i + browsingWindowOffset)
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

    uint8_t algorithm = (uint8_t) ( (rel_v - 4095) * (1 - 4) / (0 - 4095) + 4);

    switch(algorithm)
    {
        case 1:
        {
            double threshold = (att_v - 4095) * (fGP.KBlur.thrsMax - fGP.KBlur.thrsMin) / (0 - 4095) + fGP.KBlur.thrsMin;
            double gain = (dec_v - 4095) * (fGP.KBlur.gainMax - fGP.KBlur.gainMin) / (0 - 4095) + fGP.KBlur.gainMin;
            uint16_t window = (sus_v - 4095) * (fGP.KBlur.windowMax - fGP.KBlur.windowMin) / (0 - 4095) + fGP.KBlur.windowMin;
            applyKBlurForward(wave[screenstate], currentScreenWavetable, threshold , gain, window);
            fbg_write(fbg, "K-Blur-F", calcCenterOfText("K-Blur-F"), SCREEN_HEADER_Y);
        }
        break;
        case 2:
        {
            double threshold = (att_v - 4095) * (fGP.KBlur.thrsMax - fGP.KBlur.thrsMin) / (0 - 4095) + fGP.KBlur.thrsMin;
            double gain = (dec_v - 4095) * (fGP.KBlur.gainMax - fGP.KBlur.gainMin) / (0 - 4095) + fGP.KBlur.gainMin;
            uint16_t window = (sus_v - 4095) * (fGP.KBlur.windowMax - fGP.KBlur.windowMin) / (0 - 4095) + fGP.KBlur.windowMin;
            applyKBlurBackward(wave[screenstate], currentScreenWavetable, threshold , gain, window);
            fbg_write(fbg, "K-Blur-B", calcCenterOfText("K-Blur-B"), SCREEN_HEADER_Y);
        }
        break;
        case 3:
        {
            uint16_t rounds = (att_v - 4095) * (fGP.MBlur.roundsMin - fGP.MBlur.roundsMax) / (0 - 4095) + fGP.MBlur.roundsMax;
            applyMedianBlur(wave[screenstate], currentScreenWavetable, rounds);
            fbg_write(fbg, "M-Blur", calcCenterOfText("M-Blur"), SCREEN_HEADER_Y);
        }
        break;
        default:
        {
            uint16_t rounds = (att_v - 4095) * (fGP.MBlur.roundsMin - fGP.MBlur.roundsMax) / (0 - 4095) + fGP.MBlur.roundsMax;
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
    fbg_write(fbg, "Saw", SCREEN_SW4_6_POSX - 40, SCREEN_SW1_4_POSY);
    fbg_write(fbg, "Silence", SCREEN_SW4_6_POSX - 95, SCREEN_SW2_5_POSY);
    fbg_write(fbg, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    commitScreenBuffer();
}

void renderHiddenMode()
{}
