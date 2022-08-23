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
#include "Loader.hpp"

#include "GlobalDefinitions.hpp"
#include "json.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>

using json = nlohmann::json;

using namespace std;

void setupFileLoader()
{
    updateUserWavesCount();
    cout << "userWavesCount: " << fileUserWavesCount << endl;
}

void updateUserWavesCount()
{
    int64_t buffer = getFileCountInDirectory("/home/pi/WaveformMorpherWaves/") - 2;
    if(buffer >= 0)
    {
        fileUserWavesCount = buffer;
    }
    else
    {
        cout << "ISSUE WITH THE FILE READING" << endl;
        //TODO THROW BIG ERROR
        return;
    }
}

uint32_t getFileCountInDirectory(std::string path)
{
    DIR *d;
    struct dirent *dir;
    uint32_t entries = 0;

    d = opendir(path.c_str());
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            entries++;
        }
        closedir(d);
    }
    return entries;
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

void loadPatchFromFile(std::string name)
{
    std::ifstream inputFile("/home/pi/WaveformMorpherPatches/" + name /* + ".wmp"*/);
    json incomingJSON;
    inputFile >> incomingJSON;

    for(int j = 0; j < 5; j++)
    {
        for(int i = 0; i < WAVE_TABLE_SIZE; i++)
        {
            wave[j][i] = incomingJSON["wave"][j][i];
            fft[j][i] = incomingJSON["fft"][j][i];
        }
    }

    NTables = (int) incomingJSON["NTables"];

    VCAFlag = (bool) incomingJSON["VCA_FLAG"];

    KnobBehaviour = (bool) incomingJSON["KnobBehaviour"];

    knob1Value = (double) incomingJSON["att_v"];
    knobEnterHeldValue[0] = (double) incomingJSON["att_v"];
    knobState[0] = false;
    setADSRValue((double) incomingJSON["att_v"], 0);

    knob2Value = (double) incomingJSON["dec_v"];
    knobEnterHeldValue[1] = (double) incomingJSON["dec_v"];
    knobState[1] = false;
    setADSRValue((double) incomingJSON["dec_v"], 1);

    knob3Value = (double) incomingJSON["sus_v"];
    knobEnterHeldValue[2] = (double) incomingJSON["sus_v"];
    knobState[2] = false;
    setADSRValue((double) incomingJSON["sus_v"], 2);

    knob4Value = (double) incomingJSON["rel_v"];
    knobEnterHeldValue[3] = (double) incomingJSON["rel_v"];
    knobState[3] = false;
    setADSRValue((double) incomingJSON["rel_v"], 3);

    loop_v = (double) incomingJSON["loop_v"];

    screenstate = A;
    uiState = EditView;
    dynamic_view = 0;
    fourier_flag = 0;
    fft_has_been_touched_flag = 0;

    currentEditWavetable = wave[0];
}

void savePatchToFile(std::string name)
{
    json outgoingJSON = {
        {"wave", wave},
        {"fft", fft},
        {"NTables", NTables},
        {"VCA_FLAG", VCAFlag},
        {"KnobBehaviour", KnobBehaviour},
        {"att_v", knob1Value},
        {"dec_v", knob2Value},
        {"sus_v", knob3Value},
        {"rel_v", knob4Value},
        {"loop_v", loop_v},
    };

    std::ofstream outputFile("/home/pi/WaveformMorpherPatches/" + name + ".wmp");
    outputFile << std::setw(4) << outgoingJSON << std::endl; //THE SETW MAKES IT PRETTY}

void loadWaveFromFile(uint32_t index)
{
    if(fileUserWavesCount == 0)
    {
        genSil(currentScreenWavetable);
    }
    else
    {
        std::string path = std::string("/home/pi/WaveformMorpherWaves/") + std::to_string(index) + ".wmw";
        std::ifstream inputFile(path);
        json incomingJSON;
        if(inputFile.is_open())
        {
            inputFile >> incomingJSON;

            for(int i = 0; i < WAVE_TABLE_SIZE; i++)
            {
                currentScreenWavetable[i] = incomingJSON["wave"][i];
            }
        }
        else
        {
            genSil(currentScreenWavetable);
        }
    }
}

void saveWaveToFile()
{
    json outgoingJSON = {
        {"wave", wave[screenstate]},
        {"fft", fft[screenstate]},
    };

    std::string path = std::string("/home/pi/WaveformMorpherWaves/") + std::to_string(fileUserWavesCount + 1) + ".wmw";
    std::ofstream outputFile(path);
    outputFile << std::setw(4) << outgoingJSON << std::endl; //THE SETW MAKES IT PRETTY
}

void deleteWaveFile(uint32_t index)
{
    std::string path = std::string("/home/pi/WaveformMorpherWaves/") + std::to_string(index) + ".wmw";
    remove(&path[0]);
}
