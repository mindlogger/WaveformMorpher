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
        .VCA = false,
    },
};
