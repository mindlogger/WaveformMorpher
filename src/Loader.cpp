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

#include <json.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <dirent.h>

using namespace std;

void setupLoader()
{
	struct json_object *jobj;
	char *str = "{ \"msg-type\": [ \"0xdeadbeef\", \"irc log\" ], \
		\"msg-from\": { \"class\": \"soldier\", \"name\": \"Wixilav\" }, \
		\"msg-to\": { \"class\": \"supreme-commander\", \"name\": \"[Redacted]\" }, \
		\"msg-log\": [ \
			\"soldier: Boss there is a slight problem with the piece offering to humans\", \
			\"supreme-commander: Explain yourself soldier!\", \
			\"soldier: Well they don't seem to move anymore...\", \
			\"supreme-commander: Oh snap, I came here to see them twerk!\" \
			] \
		}";

	printf("str:\n---\n%s\n---\n\n", str);

	jobj = json_tokener_parse(str);
	printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
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
    cout << "LOADING: " << name << endl;
    std::string path = "/home/pi/WaveformMorpherPatches/" + name; // + ".wmp"
    FILE *fp;
    char buff[255];

    fp = fopen(&path[0], "r");

    fgets(buff, 5, (FILE*)fp);

    cout << "TILL"<< endl;

    fclose(fp);

    cout << "CONTENT: " << buff << endl;
    //JSON PARSING
}

void saveFile(std::string name)
{
    FILE *fp;
    std::string path = "/home/pi/WaveformMorpherPatches/" + name + ".wmp";

    char str[] = "DUMMY CONTENT\n";

    fp  = fopen(&path[0], "w");
    int i;

    for (i = 0; str[i] != '\n'; i++)
    {
        fputc(str[i], fp);
    }

    fclose(fp);}
