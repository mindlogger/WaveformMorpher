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
        if(fourier_flag)
        {
            switch(screenstate)
            {
                case A:
                fbg_write(fbg, "As.", 4, 2);
                break;
                case D:
                fbg_write(fbg, "Ds.", 4, 2);
                break;
                case SS:
                fbg_write(fbg, "SSs.", 4, 2);
                break;
                case SE:
                fbg_write(fbg, "SEs.", 4, 2);
                break;
                case R:
                fbg_write(fbg, "Rs.", 4, 2);
                break;
            }
        }
        else
        {
            switch(screenstate)
            {
                case A:
                fbg_write(fbg, "A.", 4, 2);
                break;
                case D:
                fbg_write(fbg, "D.", 4, 2);
                break;
                case SS:
                fbg_write(fbg, "SS.", 4, 2);
                break;
                case SE:
                fbg_write(fbg, "SE.", 4, 2);
                break;
                case R:
                fbg_write(fbg, "R.", 4, 2);
                break;
            }
        }
    }
    else
    {
        if(fourier_flag)
        {
            switch(screenstate)
            {
                case A:
                fbg_write(fbg, "As", 4, 2);
                break;
                case D:
                fbg_write(fbg, "Ds", 4, 2);
                break;
                case SS:
                fbg_write(fbg, "SSs", 4, 2);
                break;
                case SE:
                fbg_write(fbg, "SEs", 4, 2);
                break;
                case R:
                fbg_write(fbg, "Rs", 4, 2);
                break;
            }
        }
        else
        {
            switch(screenstate)
            {
                case A:
                fbg_write(fbg, "A", 4, 2);
                break;
                case D:
                fbg_write(fbg, "D", 4, 2);
                break;
                case SS:
                fbg_write(fbg, "SS", 4, 2);
                break;
                case SE:
                fbg_write(fbg, "SE", 4, 2);
                break;
                case R:
                fbg_write(fbg, "R", 4, 2);
                break;
            }
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

    fbg_write(fbg, "Patch Settings", 140, 15);
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
    fbg_write(fbg, "Load Patch", 140, 15);
    commitScreenBuffer();
}

void renderBlurMode()
{
    clearScreen();

    double threshold = (att_v - 4095) * (fGP.Blur.thrsMax - fGP.Blur.thrsMin) / (0 - 4095) + fGP.Blur.thrsMin;
    applyKBlur(wave[screenstate], currentScreenWavetable, threshold , threshold);

    fbg_write(fbg, "<", SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);
    fbg_write(fbg, ">", SCREEN_SW4_5_POSX, SCREEN_SW1_4_POSY);
    fbg_write(fbg, "Blur", 190, 15); //TODO IMPLEMENT MORE BLURING ALGORITHMS

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
}
void renderHiddenMode()
{}
