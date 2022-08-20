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
    tBlur Blur;
    tVisual Visual;
} tGlobalPreset;

extern tGlobalPreset fGP;

#endif
