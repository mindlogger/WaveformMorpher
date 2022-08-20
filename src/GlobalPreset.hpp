#ifndef GLOBALPRESET_H
#define GLOBALPRESET_H

typedef struct {
    double thrsMax;
    double thrsMin;
} tBlur;

typedef struct {
    bool continous;
} tVisual;

typedef struct {
    bool VCA;
} tSynth;

typedef struct {
    tBlur Blur;
    tVisual Visual;
    tSynth Synth;
} tGlobalPreset;

extern tGlobalPreset fGP;

#endif
