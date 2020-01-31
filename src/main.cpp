#include "AudioOut.hpp"
#include "Framebuffer.hpp"
#include "Touchscreen.hpp"
#include "WaveOsc.hpp"
#include "MidiHandling.hpp"
#include "WaveGenerator.hpp"
#include "TimerEvent.hpp"

#define SAMPLE_RATE   (48000)
#define WAVE_TABLE_SIZE (480)

using namespace std;

float screen_wave[WAVE_TABLE_SIZE]; //y = 0..150..300

int main()
{
    setupTimer();
    gensinus(screen_wave,WAVE_TABLE_SIZE);
    initFramebuffer(screen_wave);
    initAudio(screen_wave);
    initTouchscreen(screen_wave);
    initMidi();
    while(true)
    {
        loopedtouch();
    }
    return 0;
}

