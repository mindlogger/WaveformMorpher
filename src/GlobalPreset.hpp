#ifndef GLOBALPRESET_H
#define GLOBALPRESET_H
#include <stdint.h>

typedef struct {
    double thrsMax;
    double thrsMin;
    double gainMax;
    double gainMin;
    uint16_t windowMax;
    uint16_t windowMin;
} tKBlur;

typedef struct {
    uint16_t roundsMin;
    uint16_t roundsMax;
} tMBlur;

typedef struct {
    bool continous;
} tVisual;

typedef struct {
} tSynth;

typedef struct {
    tKBlur KBlur;
    tMBlur MBlur;
    tVisual Visual;
    tSynth Synth;
} tGlobalPreset;

extern tGlobalPreset fGP;

#endif
