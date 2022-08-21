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

void renderEditView()
{
    clearScreen();
    insertCurrentTableName();
    table2Screen(currentEditWavetable);
}

void renderPatchSettings()
{
    clearScreen();

    fbg_write(fbg, "Patch Settings", 140, SCREEN_HEADER_Y);
    if(VCA_FLAG)
        fbg_write(fbg, "VCA:On", SCREEN_SW4_5_POSX - 100, SCREEN_SW2_5_POSY);
    else
        fbg_write(fbg, "VCA:Off", SCREEN_SW4_5_POSX - 100, SCREEN_SW2_5_POSY);
    fbg_write(fbg, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    commitScreenBuffer();
}

void renderGlobalSettings()
{
    clearScreen();

    fbg_write(fbg, "Global Settings", 140, 15);
    fbg_write(fbg, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    commitScreenBuffer();
}

void renderLoad()
{
    clearScreen();

    for(int i = 0; i < 9 && i < filesInDirectory.size(); i++)
    {
        if(fileSelectionIndex == i + browsingWindowOffset)
        {
            fbg_write(fbg, "o", 110, 50 + (i*30));
        }
        fbg_write(fbg, &filesInDirectory[i + browsingWindowOffset][0], 140, 50 + (i*30));
    }
    fbg_write(fbg, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    fbg_write(fbg, ">", SCREEN_SW4_5_POSX, SCREEN_SW1_4_POSY);
    fbg_write(fbg, "Load Patch", 140, SCREEN_HEADER_Y);
    commitScreenBuffer();
}

void renderBlurMode()
{
    clearScreen();

    fbg_write(fbg, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    fbg_write(fbg, ">", SCREEN_SW4_5_POSX, SCREEN_SW1_4_POSY);

    uint8_t algorithm = (uint8_t) ( (rel_v - 4095) * (1 - 4) / (0 - 4095) + 4);

    switch(algorithm)
    {
        case 1:
        {
            double threshold = (att_v - 4095) * (fGP.KBlur.thrsMax - fGP.KBlur.thrsMin) / (0 - 4095) + fGP.KBlur.thrsMin;
            double gain = (dec_v - 4095) * (fGP.KBlur.gainMax - fGP.KBlur.gainMin) / (0 - 4095) + fGP.KBlur.gainMin;
            uint16_t window = (sus_v - 4095) * (fGP.KBlur.windowMax - fGP.KBlur.windowMin) / (0 - 4095) + fGP.KBlur.windowMin;
            applyKBlurForward(wave[screenstate], currentScreenWavetable, threshold , gain, window);
            fbg_write(fbg, "K-Blur-F", 190, 15);
        }
        break;
        case 2:
        {
            double threshold = (att_v - 4095) * (fGP.KBlur.thrsMax - fGP.KBlur.thrsMin) / (0 - 4095) + fGP.KBlur.thrsMin;
            double gain = (dec_v - 4095) * (fGP.KBlur.gainMax - fGP.KBlur.gainMin) / (0 - 4095) + fGP.KBlur.gainMin;
            uint16_t window = (sus_v - 4095) * (fGP.KBlur.windowMax - fGP.KBlur.windowMin) / (0 - 4095) + fGP.KBlur.windowMin;
            applyKBlurBackward(wave[screenstate], currentScreenWavetable, threshold , gain, window);
            fbg_write(fbg, "K-Blur-B", 190, 15);
        }
        break;
        case 3:
        {
            uint16_t rounds = (att_v - 4095) * (fGP.MBlur.roundsMin - fGP.MBlur.roundsMax) / (0 - 4095) + fGP.MBlur.roundsMax;
            applyMedianBlur(wave[screenstate], currentScreenWavetable, rounds);
            fbg_write(fbg, "M-Blur", 190, 15);
        }
        break;
        default:
        {
            uint16_t rounds = (att_v - 4095) * (fGP.MBlur.roundsMin - fGP.MBlur.roundsMax) / (0 - 4095) + fGP.MBlur.roundsMax;
            applyMedianBlur(wave[screenstate], currentScreenWavetable, rounds);
            fbg_write(fbg, "M-Blur", 190, 15);
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
        cout << patchName << endl;
        //DO INIT SHIT
        for(int i = 0; i < 12; i++)
        {
            fileSaveCharacters[i] = characterUpercaseOptions[rand() % 39];
        }
        //USE characterUpercaseOptions
    }
    else
    {
        fbg_write(fbg, patchName, 140, 40);

        for(int i = 0; i < 12; i++)
        {
            fileSaveCharacters[i] = characterOptions[rand() % 39];
        }
    }

    //FIND THE RANDOMLY ASSIGNED LETTERS STORE + DISPLAY THEM

    fbg_write(fbg, "Store Patch", 140, 15);

    SW1Text[0] = fileSaveCharacters[0];
    SW1Text[1] = '/';
    SW1Text[2] = fileSaveCharacters[6];

    SW2Text[0] = fileSaveCharacters[1];
    SW2Text[1] = '/';
    SW2Text[2] = fileSaveCharacters[7];

    SW3Text[0] = '<';
    SW3Text[1] = '/';
    SW3Text[2] = fileSaveCharacters[8];

    SW4Text[0] = '.';
    SW4Text[1] = '/';
    SW4Text[2] = '>';

    SW5Text[0] = fileSaveCharacters[4];
    SW5Text[1] = '/';
    SW5Text[2] = fileSaveCharacters[10];

    SW6Text[0] = fileSaveCharacters[5];
    SW6Text[1] = '/';
    SW6Text[2] = fileSaveCharacters[11];

    fbg_write(fbg, SW1Text, SCREEN_SW1_3_POSX, SCREEN_SW1_4_POSY);
    fbg_write(fbg, SW2Text, SCREEN_SW1_3_POSX, SCREEN_SW2_5_POSY);
    fbg_write(fbg, SW3Text, SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    fbg_write(fbg, SW4Text, SCREEN_SW4_5_POSX - 33, SCREEN_SW1_4_POSY);
    fbg_write(fbg, SW5Text, SCREEN_SW4_5_POSX - 33, SCREEN_SW2_5_POSY);
    fbg_write(fbg, SW6Text, SCREEN_SW4_5_POSX - 33, SCREEN_SW3_6_POSY);

    commitScreenBuffer();
}
void renderInsertWave()
{
    clearScreen();
    fbg_write(fbg, "Insert Wave", 140, SCREEN_HEADER_Y);
    fbg_write(fbg, "Sine", SCREEN_SW1_3_POSX, SCREEN_SW1_4_POSY);
    fbg_write(fbg, "Square", SCREEN_SW1_3_POSX, SCREEN_SW2_5_POSY);
    fbg_write(fbg, "Saw", SCREEN_SW4_5_POSX - 40, SCREEN_SW1_4_POSY);
    fbg_write(fbg, "Silence", SCREEN_SW4_5_POSX - 95, SCREEN_SW2_5_POSY);
    fbg_write(fbg, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    commitScreenBuffer();
}
void renderHiddenMode()
{}
