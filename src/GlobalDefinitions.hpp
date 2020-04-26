#define SAMPLE_RATE (48000)
#define WAVE_TABLE_SIZE (480)
#ifdef GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif
#include "ADSR.hpp"
enum Screenstates {A,D,SS,SE,R};
EXTERN int fourier_flag; //0 = wave 1 = spectrum
EXTERN Screenstates screenstate;
//TODO MAKE EXTERN STRUCT THAT HAS ALL THE IMPORTANT CURRENT STATES
EXTERN float master_gain; //0 -- 1
EXTERN int touch_is_happening_flag;
EXTERN int fft_has_been_touched_flag;
EXTERN int dynamic_view;
EXTERN int n_shutdown_flag;
EXTERN ADSR *envelope;

EXTERN double currentScreenWavetable[WAVE_TABLE_SIZE];
EXTERN double *currentEditWavetable;

EXTERN double wave[5][WAVE_TABLE_SIZE];
EXTERN double fft[5][WAVE_TABLE_SIZE];

EXTERN double att_v;
EXTERN double dec_v;
EXTERN double sus_v;
EXTERN double rel_v;
EXTERN double loop_v;
