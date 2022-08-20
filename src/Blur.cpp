#include "GlobalDefinitions.hpp"
#include "Blur.hpp"
#include <math.h>


void applyKBlur(double *sourceTable, double *destinationTable, double threshold, double offset) //TODO IMPROVE THE PERFORMANCE OF THIS
{
    for(int i = 0; i < WAVE_TABLE_SIZE; i++)
    {
        destinationTable[i] = sourceTable[i];
    }
    for(int i = 0; i < WAVE_TABLE_SIZE - 1; i++)
    {
        if(destinationTable[i] + threshold < destinationTable[i + 1])
        {
            //TO BIG LEAP UP
            destinationTable[i+1] = destinationTable[i] + offset;
        }
        else if(destinationTable[i] - threshold > destinationTable[i + 1])
        {
            //TO BIG LEAP DOWN
            destinationTable[i+1] = destinationTable[i] - offset;
        }
    }
}
void applyGaussianBlur(double *sourceTable, double *destinationTable)
{
    //TODO
}
