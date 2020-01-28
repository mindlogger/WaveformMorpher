#include "AudioOut.hpp"
#include "Framebuffer.hpp"
#include "Touchscreen.hpp"
#include "WaveOsc.hpp"

#define SAMPLE_RATE   (48000)
#define WAVE_TABLE_SIZE (480)


using namespace std;

int screen_wave[480]; //y = 0..150..300

int main()
{
    initFramebuffer(screen_wave);
    initAudio(screen_wave);
    initTouchscreen(screen_wave);
    while(true)
    {
        loopedtouch();
    }
    return 0;
}
