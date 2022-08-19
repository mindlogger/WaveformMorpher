#include "FbGraphics.hpp"
#include "GlobalDefinitions.hpp"

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
void renderStore()
{
    clearScreen();

    fbg_write(fbg, "Store Patch", 140, 15);
    commitScreenBuffer();
}
void renderInsertWave()
{
}
void renderHiddenMode()
{}
