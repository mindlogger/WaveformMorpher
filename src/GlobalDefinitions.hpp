#define SAMPLE_RATE (48000)
#define WAVE_TABLE_SIZE (480)
#ifdef GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

enum class Screenstates {A_W,SS_W,SE_W,R_W, A_F,SS_F,SE_F,R_F};
EXTERN Screenstates screenstate;
