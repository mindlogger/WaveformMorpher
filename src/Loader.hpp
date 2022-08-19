#ifndef LOADER_H
#define LOADER_H

#include <string>

void setupLoader();
void getFilesInDirectory();
void loadFile(std::string name);
void loadFilePreset();
void loadFileGlobal();
void saveFile(std::string name);

#endif
