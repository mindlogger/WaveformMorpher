#ifndef LOADER_H
#define LOADER_H

#include <string>

void setupFileLoader();
void updateUserWavesCount();
uint32_t getFileCountInDirectory(std::string path);
void getFilesInDirectory();
void loadPatchFromFile(std::string name);
void savePatchToFile(std::string name);

void loadWaveFromFile(uint32_t index);
void saveWaveToFile();
void deleteWaveFile(uint32_t index);

void loadFilePreset();
void loadFileGlobal();

#endif
