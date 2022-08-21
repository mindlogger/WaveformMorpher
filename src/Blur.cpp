#include "GlobalDefinitions.hpp"
#include "Blur.hpp"
#include <iostream>
#include <math.h>

using namespace std;

void applyKBlurForward(double *sourceTable, double *destinationTable, double threshold, double offset, uint16_t window) //TODO IMPROVE THE PERFORMANCE OF THIS
{
    for(int i = 0; i < WAVE_TABLE_SIZE; i++) //COPY THE WAVETABLE SO THERE IS NO LIVE EDITING
    {
        destinationTable[i] = sourceTable[i];
    }

    for(int i = 0; i < WAVE_TABLE_SIZE; i++) //TODO ADD  + window ??
    {
        for(int w = 1; w < window; w++)
        {
            if(destinationTable[i] + threshold < destinationTable[(i + w) % WAVE_TABLE_SIZE ])//TO BIG LEAP UP
            {
                destinationTable[(i + w) % WAVE_TABLE_SIZE ] = destinationTable[i] + (offset * ( (double) w) / ( (double) window));
            }
            else if(destinationTable[i] - threshold > destinationTable[(i + w) % WAVE_TABLE_SIZE ])//TO BIG LEAP DOWN
            {
                destinationTable[(i + w) % WAVE_TABLE_SIZE ] = destinationTable[i] - (offset * ( (double) w) / ( (double) window));
            }
        }
    }
}

void applyKBlurBackward(double *sourceTable, double *destinationTable, double threshold, double offset, uint16_t window) //TODO IMPROVE THE PERFORMANCE OF THIS
{
    for(int i = 0; i < WAVE_TABLE_SIZE; i++) //COPY THE WAVETABLE SO THERE IS NO LIVE EDITING
    {
        destinationTable[i] = sourceTable[i];
    }

    for(int i = 0; i < WAVE_TABLE_SIZE + window; i++)
    {
        for(int w = 1; w < window; w++)
        {
            int whereToLook = i - w;
            if(whereToLook < 0)
            {
                whereToLook = WAVE_TABLE_SIZE - whereToLook;
            }
            whereToLook = abs(whereToLook);
            whereToLook = whereToLook % WAVE_TABLE_SIZE;
            if(destinationTable[i] + threshold < destinationTable[whereToLook])//TO BIG LEAP UP
            {
                destinationTable[whereToLook] = destinationTable[i] - (offset * ( (double) w) / ( (double) window));
            }
            else if(destinationTable[i] - threshold > destinationTable[whereToLook])//TO BIG LEAP DOWN
            {
                destinationTable[whereToLook] = destinationTable[i] + (offset * ( (double) w) / ( (double) window));
            }
        }
    }
}

void applyMedianBlur(double *sourceTable, double *destinationTable, uint16_t rounds)
{
    for(int i = 0; i < WAVE_TABLE_SIZE; i++) //COPY THE WAVETABLE SO THERE IS NO LIVE EDITING
    {
        destinationTable[i] = sourceTable[i];
    }
    for(int j = 0; j < rounds; j++)
    {
        for(int i = 0; i < WAVE_TABLE_SIZE + rounds; i++)
        {
            destinationTable[(i + 1 ) % (WAVE_TABLE_SIZE - 1)] = (destinationTable[i % WAVE_TABLE_SIZE] + destinationTable[(i + 1) % (WAVE_TABLE_SIZE - 1)]) / 2;
        }
        for(int i = 0; i < rounds; i++)
        {
            destinationTable[(i + 1 ) % (WAVE_TABLE_SIZE - 1)] = (destinationTable[i % WAVE_TABLE_SIZE] + destinationTable[(i + 1) % (WAVE_TABLE_SIZE - 1)]) / 2;
        }
    }
}

void applyGaussianBlur(double *sourceTable, double *destinationTable)
{
    //TODO
}
