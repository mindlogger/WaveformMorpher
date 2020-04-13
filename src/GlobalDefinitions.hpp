#define SAMPLE_RATE (48000)
#define WAVE_TABLE_SIZE (480)
#ifdef GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif
#include "ADSR.hpp"
enum class Screenstates {A_W,D_W,SS_W,SE_W,R_W, A_F,D_F,SS_F,SE_F,R_F}; //W = Wave F = Fourier
EXTERN Screenstates screenstate;
//MAKE EXTERN STRUCT THAT HAS ALL THE IMPORTANT CURRENT STATES
EXTERN int dynamic_view;
EXTERN float master_gain; //0 -- 1
EXTERN int touch_is_happening_flag;
EXTERN ADSR *env;

EXTERN double *audioOutWavetable;
EXTERN double *currentScreenWavetable;
EXTERN double *currentEditWavetable;
EXTERN double mainWave[WAVE_TABLE_SIZE]; //-1 -- 0 -- 1
EXTERN double wave[5][WAVE_TABLE_SIZE];
EXTERN double mainFFT[WAVE_TABLE_SIZE]; //-1 -- 0 -- 1
