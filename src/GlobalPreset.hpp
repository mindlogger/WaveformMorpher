#ifndef GLOBALPRESET_H
#define GLOBALPRESET_H

typedef struct {
    double thrsMax;
    double thrsMin;
} tBlur;

typedef struct {
    tBlur Blur;
} tGlobalPreset;

extern const tGlobalPreset fGP;

#endif
