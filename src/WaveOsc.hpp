// WaveOsc.h
#ifndef WAVEOSC_H
#define WAVEOSC_H
float getWavetableValue();
void setfreq(double f);
enum envState {
    env_idle = 0,
    env_attack,
    env_decay,
    env_sustain,
    env_release
};
#endif

