// WaveOsc.h
#ifndef WAVEOSC_H
#define WAVEOSC_H
void setNTables();
float getWavetableValue();
void setfreq(double f);
enum envState {
    env_idl = 0,
    env_attack,
    env_decay,
    env_sustain,
    env_release
};
#endif

