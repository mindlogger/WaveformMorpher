#include "GlobalDefinitions.hpp"
#include "Blur.hpp"
void applyKBlur(double *table, double threshold, double offset)
{
    for(int i = 0; i < WAVE_TABLE_SIZE - 1; i++)
    {
        if(table[i] + threshold < table[i + 1])
        {
            //TO BIG LEAP UP
            table[i+1] = table[i] + offset;
        }
        else if(table[i] - threshold > table[i + 1])
        {
            //TO BIG LEAP DOWN
            table[i+1] = table[i] - offset;
        }

    }
}
