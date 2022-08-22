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
    uint8_t r;
    uint8_t g;
    uint8_t b;
} tRGB;

typedef struct {
    tRGB Confirm;
    tRGB Cancel;
    tRGB HighlightA;
} tColor;

enum EKnobResponseTypes {EditViewOnly, PerPatch, Always};

typedef struct {
    tKBlur KBlur;
    tMBlur MBlur;
    tVisual Visual;
    tSynth Synth;
    EKnobResponseTypes KnobResponse;
    tColor Color;
} tGlobalPreset;

extern tGlobalPreset fGP;

#endif
