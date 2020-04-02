#define SAMPLE_RATE (48000)
#define WAVE_TABLE_SIZE (480)
#ifdef GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

enum class Screenstates {A_W,D_W,SS_W,SE_W,R_W, A_F,D_F,SS_F,SE_F,R_F}; //W = Wave F = Fourier
enum class ADSRstates {A_D,D_SS,SS_SE,SE_R};
EXTERN Screenstates screenstate;
EXTERN ADSRstates adsrstate;
EXTERN float master_gain; //0 -- 1

EXTERN double *audioOutWavetable;
EXTERN double *currentScreenWavetable;
EXTERN double mainWave[WAVE_TABLE_SIZE]; //-1 -- 0 -- 1
EXTERN double mainFFT[WAVE_TABLE_SIZE]; //-1 -- 0 -- 1
