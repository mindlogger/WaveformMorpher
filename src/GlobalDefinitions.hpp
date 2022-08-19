#define SAMPLE_RATE (48000)
#define WAVE_TABLE_SIZE (480)
#define SCREEN_SW1_3_POSX 4
#define SCREEN_SW4_5_POSX 427
#define SCREEN_SW1_4_POSY 31
#define SCREEN_SW2_5_POSY 148
#define SCREEN_SW3_6_POSY 260
#ifdef GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif
#include "ADSR.hpp"
#include <stdint.h>
#include <vector>
#include <string>

enum Screenstates {A,D,SS,SE,R}; //PS: Patch Settings //GS: Global Settings
enum UIStates {EditView, DynamicView, PatchSettings, GlobalSettings, Load, Store, InsertWave, HiddenMode};
EXTERN int fourier_flag; //0 = wave 1 = spectrum
EXTERN Screenstates screenstate;
EXTERN UIStates uiState;
EXTERN void (*SW1Event)(uint32_t tick, uint8_t id);
EXTERN void (*SW1ShiftEvent)(uint32_t tick, uint8_t id);
EXTERN void (*SW2Event)(uint32_t tick, uint8_t id);
EXTERN void (*SW2ShiftEvent)(uint32_t tick, uint8_t id);
EXTERN void (*SW3Event)(uint32_t tick, uint8_t id);
EXTERN void (*SW3ShiftEvent)(uint32_t tick, uint8_t id);
EXTERN void (*SW4Event)(uint32_t tick, uint8_t id);
EXTERN void (*SW4ShiftEvent)(uint32_t tick, uint8_t id);
EXTERN void (*SW5Event)(uint32_t tick, uint8_t id);
EXTERN void (*SW5ShiftEvent)(uint32_t tick, uint8_t id);
EXTERN void (*SW6Event)(uint32_t tick, uint8_t id);
EXTERN void (*SW6ShiftEvent)(uint32_t tick, uint8_t id);

EXTERN struct _fbg *fbg;
EXTERN struct _fbg_img *bb_font_img;
EXTERN struct _fbg_font *bbfont;

EXTERN char fileSaveCharacters[12];
EXTERN int patchNameIndex;
EXTERN char* patchName;
EXTERN uint8_t fileSelectionIndex;

EXTERN std::vector<std::string> filesInDirectory;

//EXTERN void (*ScreenUIDrawer)(void);
EXTERN int NTables;
EXTERN uint8_t browsingWindowOffset;
//TODO MAKE EXTERN STRUCT THAT HAS ALL THE IMPORTANT CURRENT STATES
EXTERN float master_gain; //0 -- 1
EXTERN int touch_is_happening_flag;
EXTERN int fft_has_been_touched_flag;
EXTERN int dynamic_view;
EXTERN int n_shutdown_flag;
EXTERN int shift_flag;
EXTERN ADSR *envelope;

EXTERN double currentScreenWavetable[WAVE_TABLE_SIZE]; //FOR DYNAMIC VIEW RENDERING
EXTERN double *currentEditWavetable;

EXTERN double wave[5][WAVE_TABLE_SIZE];
EXTERN double fft[5][WAVE_TABLE_SIZE];

EXTERN double clipboard[WAVE_TABLE_SIZE];

EXTERN double att_v;
EXTERN double dec_v;
EXTERN double sus_v;
EXTERN double rel_v;
EXTERN double loop_v;
