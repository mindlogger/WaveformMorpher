#include "FbGraphics.hpp"
#include "GlobalDefinitions.hpp"

#include "math.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

extern "C"
{
#include "fbgraphics.h"
}

void renderEditView()
{
    clearScreen();

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

    table2Screen(currentEditWavetable);
}
void renderPatchSettings()
{
    clearScreen();

    fbg_write(fbg, "Patch Settings", 140, 15);
    commitScreenBuffer();
}
void renderGlobalSettings()
{
    clearScreen();

    fbg_write(fbg, "Global Settings", 140, 15);
    commitScreenBuffer();
}
void renderLoad()
{
    clearScreen();

    fbg_write(fbg, "Load Patch", 140, 15);
    commitScreenBuffer();
}

#define MAX_PATCH_NAME_LENGTH 16
char characterUpercaseOptions[39] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_-"; //ACTUALLY ONLY 38 CHARACTERS
char characterOptions[39] = "abcdefghijklmnopqrstuvwxyz1234567890_-"; //ACTUALLY ONLY 38 CHARACTERS

char SW1Text[3];
char SW2Text[3];
char SW3Text[3];

char SW5Text[3];
char SW6Text[3];

void renderStore()
{
    clearScreen();

    if(patchNameIndex == 0)
    {
        patchName = (char*)malloc(MAX_PATCH_NAME_LENGTH);
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

    SW3Text[0] = ' ';
    SW3Text[1] = '/';
    SW3Text[2] = fileSaveCharacters[8];

    SW5Text[0] = fileSaveCharacters[4];
    SW5Text[1] = '/';
    SW5Text[2] = fileSaveCharacters[10];

    SW6Text[0] = fileSaveCharacters[5];
    SW6Text[1] = '/';
    SW6Text[2] = fileSaveCharacters[11];

    fbg_write(fbg, SW1Text, SCREEN_SW1_3_POSX, SCREEN_SW1_4_POSY);
    fbg_write(fbg, SW2Text, SCREEN_SW1_3_POSX, SCREEN_SW2_5_POSY);
    fbg_write(fbg, SW3Text, SCREEN_SW1_3_POSX, SCREEN_SW3_6_POSY);

    //TODO DRAW ARROW BACK SW3
    //TODO DRAW DICE / SAVE IMAGE

    fbg_write(fbg, SW5Text, SCREEN_SW4_5_POSX, SCREEN_SW2_5_POSY);
    fbg_write(fbg, SW6Text, SCREEN_SW4_5_POSX, SCREEN_SW3_6_POSY);

    commitScreenBuffer();
}
void renderInsertWave()
{
}
void renderHiddenMode()
{}
