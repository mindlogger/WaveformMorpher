#define SAMPLE_RATE (48000)
#define WAVE_TABLE_SIZE (480)

#define SCREEN_SW1_3_POSX 4
#define SCREEN_SW4_6_POSX 460

#define SCREEN_SW1_4_POSY 31
#define SCREEN_SW2_5_POSY 148
#define SCREEN_SW3_6_POSY 260

#define SCREEN_HEADER_Y 7
#define SCREEN_SECOND_HEADER_Y 33

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
enum UIStates {EditView, DynamicView, PatchSettings, GlobalSettings, Load, Store, InsertWave, BlurMode, HiddenMode};
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

/*PATCH SETTINGS START*/
EXTERN double wave[5][WAVE_TABLE_SIZE];
EXTERN double fft[5][WAVE_TABLE_SIZE];

EXTERN int NTables;
EXTERN bool VCAFlag;
EXTERN bool KnobBehaviour; // false:= always readout; true:= edit view only

EXTERN double knob1Value;
EXTERN double knob2Value;
EXTERN double knob3Value;
EXTERN double knob4Value;
EXTERN double loop_v;
/*PATCH SETTINGS END*/

EXTERN uint8_t browsingWindowOffset;
EXTERN uint8_t loopingFlag;

EXTERN double master_gain; //0 -- 1
EXTERN int touch_is_happening_flag;
EXTERN int fft_has_been_touched_flag;
EXTERN int dynamic_view;
EXTERN int n_shutdown_flag;
EXTERN int shift_flag;
EXTERN ADSR *envelope;

EXTERN double HELPVALUE;

EXTERN double currentScreenWavetable[WAVE_TABLE_SIZE];
EXTERN double *currentEditWavetable;

EXTERN double clipboard[WAVE_TABLE_SIZE];

EXTERN double fftEnterSnapshot[WAVE_TABLE_SIZE];

EXTERN double* AttackTable;
EXTERN double* DecayTable;
EXTERN double* SustainStartTable;
EXTERN double* SustainEndTable;
EXTERN double* ReleaseTable;

EXTERN double* WaveTable[5];


