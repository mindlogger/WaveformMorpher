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
            .r = 175,
            .g = 238,
            .b = 238,
        },
    },
};
