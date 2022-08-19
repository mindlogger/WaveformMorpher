#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <vector>

void setupLoader();
std::vector<std::string> getFilesInDirectory();
void loadFilePreset();
void loadFileGlobal();
void saveFile(std::string name);

#endif
