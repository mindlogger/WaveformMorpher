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

#include "GlobalDefinitions.hpp"
#include "json.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>

using json = nlohmann::json;


using namespace std;

void setupLoader()
{
    //SETUP THE FILE LOADER
}

void getFilesInDirectory()
{
    DIR *d;
    struct dirent *dir;

    filesInDirectory.clear();

    d = opendir("/home/pi/WaveformMorpherPatches/");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            filesInDirectory.push_back(string(dir->d_name));
        }
        closedir(d);
    }
}

void loadFile(std::string name)
{
    std::ifstream inputFile("/home/pi/WaveformMorpherPatches/" + name /* + ".wmp"*/);
    json incomingJSON;
    inputFile >> incomingJSON;

    NTables = (int) incomingJSON["NTables"];

    for(int j = 0; j < 5; j++)
    {
        for(int i = 0; i < WAVE_TABLE_SIZE; i++)
        {
            wave[j][i] = incomingJSON["wave"][j][i];
            fft[j][i] = incomingJSON["fft"][j][i];
        }
    }

    for(int i = 0; i < WAVE_TABLE_SIZE; i++)
    {
        clipboard[i] = incomingJSON["clipboard"][i];
    }

    att_v = (double) incomingJSON["att_v"];
    dec_v = (double) incomingJSON["dec_v"];
    sus_v = (double) incomingJSON["sus_v"];
    rel_v = (double) incomingJSON["rel_v"];
    loop_v= (double) incomingJSON["loop_v"];

    screenstate = A;
    uiState = EditView;
    dynamic_view = 0;
    fourier_flag = 0;
    fft_has_been_touched_flag = 0;

    currentEditWavetable = wave[0];
}

void saveFile(std::string name)
{
    json outgoingJSON = {
        {"NTables", NTables},
        {"wave", wave},
        {"fft", fft},
        {"clipboard", clipboard},
        {"att_v", att_v},
        {"dec_v", dec_v},
        {"sus_v", sus_v},
        {"rel_v", rel_v},
        {"loop_v", loop_v},
    };

    std::ofstream outputFile("/home/pi/WaveformMorpherPatches/" + name + ".wmp");
    outputFile << std::setw(4) << outgoingJSON << std::endl; //THE SETW MAKES IT PRETTY}
