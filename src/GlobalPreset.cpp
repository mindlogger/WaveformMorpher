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
    .WEnv = {
        .A = {
            .pMax = 5,
            .pMin = 0.001,
            .pRatio = 100, //.0001 to .01 for mostly-exponential and 100 is linear
        },
        .D = {
            .pMax = 5,
            .pMin = 0.001,
            .pRatio = 100, //.0001 to .01 for mostly-exponential and 100 is linear
        },
        .R = {
            .pMax = 5,
            .pMin = 0.001,
            .pRatio = 100, //UNUSED CUS ADSR ISNT READY
        },
    },
    .AEnv = {
        .A = {
            .pMax = 5,
            .pMin = 0.001,
            .pRatio = .001, //.0001 to .01 for mostly-exponential and 100 is linear
        },
        .D = {
            .pMax = 5,
            .pMin = 0.001,
            .pRatio = 100, //.0001 to .01 for mostly-exponential and 100 is linear
        },
        .R = {
            .pMax = 5,
            .pMin = 0.001,
            .pRatio = 100, //UNUSED CUS ADSR ISNT READY
        },
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
