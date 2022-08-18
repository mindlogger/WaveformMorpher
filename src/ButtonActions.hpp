#ifndef ButtonActions_H
#define ButtonActions_H

void assignMainActions();
void assingFileLoadingActions();
void assignPatchSettingActions();
void assignGlobalSettingActions();
void clearAllActions();

void dummyAction(uint32_t tick);

void actionLoad(uint32_t tick);
void actionStore(uint32_t tick);

void actionWaveStep(uint32_t tick);
void actionWaveN(uint32_t tick);
void actionExit(uint32_t tick);

void actionOpenPatchSettings(uint32_t tick);
void actionOpenGlobalSettings(uint32_t tick);
void actionExit(uint32_t tick);

void actionQuestion(uint32_t tick);
void actionQuestionS(uint32_t tick);

void actionCopy(uint32_t tick);
void actionPaste(uint32_t tick);

void actionFourier(uint32_t tick);
void actionInverse(uint32_t tick);

#endif
