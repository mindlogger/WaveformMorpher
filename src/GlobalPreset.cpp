#include "GlobalPreset.hpp"

tGlobalPreset fGP = {
    .KBlur = {
        .thrsMax = 0.7,
        .thrsMin = 0.01,
        .gainMax = 0.7,
        .gainMin = 0.01,
        .windowMax = 1,
        .windowMin = 40,
    },
    .MBlur = {
        .roundsMin = 1,
        .roundsMax = 90,
    },
    .Visual = {
        .continous = false,
    },
    .Synth = {
    },
    .UI = {
        .KnobResponse = EditViewOnly,
        .KnobWindow = 100,
    },
    .Color = {
        .Confirm = {
            .r = 0,
            .g = 255,
            .b = 0,
        },
        .Cancel = {
            .r = 255,
            .g = 0,
            .b = 0,
        },
        .HighlightA = {
            .r = 145,
            .g = 175,
            .b = 255,
        },
    },
};
