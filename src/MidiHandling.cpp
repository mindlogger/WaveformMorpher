#include "MidiHandling.hpp"
#include "WaveOsc.hpp"
#include "GlobalDefinitions.hpp"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "RtMidi.h"

RtMidiIn *midiin;

double MidiNote2Freq[120] = {16.35,
17.32,
18.35,
19.45,
20.60,
21.83,
23.12,
24.50,
25.96,
27.50,
29.14,
30.87,
32.70,
34.65,
36.71,
38.89,
41.20,
43.65,
46.25,
49.00,
51.91,
55.00,
58.27,
61.74,
65.41,
69.30,
73.42,
77.78,
82.41,
87.31,
92.50,
98.00,
103.83,
110.00,
116.54,
123.47,
130.81,
138.59,
146.83,
155.56,
164.81,
174.61,
185.00,
196.00,
207.65,
220.00,
233.08,
246.94,
261.63,
277.18,
293.66,
311.13,
329.63,
349.23,
369.99,
392.00,
415.30,
439.60,
466.16,
493.88,
523.25,
554.37,
587.33,
622.25,
659.25,
698.46,
739.99,
783.99,
830.61,
880.00,
932.33,
987.77,
1046.50,
1108.73,
1174.66,
1244.51,
1318.51,
1396.91,
1479.98,
1567.98,
1661.22,
1760.00,
1864.66,
1975.53,
2093.00,
2217.46,
2349.32,
2489.02,
2637.02,
2793.83,
2959.96,
3135.96,
3322.44,
3520.00,
3729.31,
3951.07,
4186.01,
4434.92,
4698.63,
4978.03,
5274.04,
5587.65,
5919.91,
6271.93,
6644.88,
7040.00,
7458.62,
7902.13
};

std::vector<unsigned char> noteHistory;

void midiCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
    {
        switch ((int)message->at(i))
        {
        case 144:{
                    i++; //GET NOTE DATA

                    double f = MidiNote2Freq[(int)message->at(i)];
                    noteHistory.push_back(message->at(i));
                    setfreq(f);
                    WEnv->gate(true);
                    AEnv->gate(true);

                    i++; //SKIP VELOCITY DATA
                }
        break;
        case 128:{
                    i++;//GET NOTE DATA

                    std::vector<unsigned char>::iterator releasedNoteIterator = std::find(noteHistory.begin(), noteHistory.end(), (int) message->at(i));
                    for(int i = 0; i < noteHistory.size();i++)
                    {
                        if(*releasedNoteIterator == (unsigned char) noteHistory[i])
                        {
                            noteHistory.erase(releasedNoteIterator); //ERASE THE RELEASED NOTE
                        }
                    }

                    if(noteHistory.empty())
                    {
                        WEnv->gate(false); //TURN GATE OFF IF NO NOTES LEFT
                        AEnv->gate(false);
                    }
                    else
                    {
                        double f = MidiNote2Freq[noteHistory.back()]; //SWITCH TO NEW NOTE
                        setfreq(f);
                    }

                    i++;//SKIP VELOCITY DATA
                }
        break;
        default:
        break;
        }
    }
}

void allMidiNotesOff()
{
    WEnv->gate(false); //TURN GATE OFF
    AEnv->gate(false); //TURN GATE OFF
    noteHistory.clear();
}

void initMidi()
{
    midiin = new RtMidiIn();
    // Check available ports.
    unsigned int nPorts = midiin->getPortCount();
    if ( nPorts < 2 )
    {
        std::cout << "No ports available!\n";
        endMidi();
    }
    else
    {
        midiin->openPort( 1 );
        // Set our callback function.  This should be done immediately after
        // opening the port to avoid having incoming messages written to the
        // queue.
        midiin->setCallback( &midiCallback );
        // Don't ignore sysex, timing, or active sensing messages.
        midiin->ignoreTypes( false, false, false );
        std::cout << "Midi setup done.\n";
    }
}

void endMidi()
{
    delete midiin;
}
