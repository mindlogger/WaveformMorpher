#ifndef BUTTONACTIONS_H
#define BUTTONACTIONS_H

void assignMainActions();
void assignLoadPatchActions();
void assignStoreActions();
void assignPatchSettingActions();
void assignInsertActions();
void assignGlobalSettingActions();
void assignBlurActions();

void clearAllActions();

void actionInsertWave(uint32_t tick, uint8_t id);

void dummyAction(uint32_t tick, uint8_t id);

void actionApplyBlur(uint32_t tick, uint8_t id);
void actionUndoBlur(uint32_t tick, uint8_t id);

void actionToggleKnobBehaviourPatch(uint32_t tick, uint8_t id);
void actionToggleKnobBehaviourGlobal(uint32_t tick, uint8_t id);
void actionToggleContinous(uint32_t tick, uint8_t id);

void actionCharacterClick(uint32_t tick, uint8_t id);
void actionRerollCharacters(uint32_t tick, uint8_t id);
void actionSavePatch(uint32_t tick, uint8_t id);

void actionToggleVCA(uint32_t tick, uint8_t id);

void actionBrowsePatch(uint32_t tick, uint8_t id);
void actionLoadPatch(uint32_t tick, uint8_t id);
void actionPatchBrowseUp(uint32_t tick, uint8_t id);
void actionPatchBrowseDown(uint32_t tick, uint8_t id);

void actionStorePatch(uint32_t tick, uint8_t id);

void actionStoreWave(uint32_t tick, uint8_t id);

void actionBrowseWave(uint32_t tick, uint8_t id);
void actionLoadWave(uint32_t tick, uint8_t id);
void actionDeleteWave(uint32_t tick, uint8_t id);
void actionWaveBrowseUp(uint32_t tick, uint8_t id);
void actionWaveBrowseDown(uint32_t tick, uint8_t id);

void actionWaveStep(uint32_t tick, uint8_t id);
void actionWaveN(uint32_t tick, uint8_t id);
void actionExit(uint32_t tick, uint8_t id);

void actionOpenPatchSettings(uint32_t tick, uint8_t id);
void actionOpenGlobalSettings(uint32_t tick, uint8_t id);
void actionExit(uint32_t tick, uint8_t id);

void actionQuestion(uint32_t tick, uint8_t id);
void actionQuestionS(uint32_t tick, uint8_t id);

void actionCopy(uint32_t tick, uint8_t id);
void actionPaste(uint32_t tick, uint8_t id);

void actionFourier(uint32_t tick, uint8_t id);
void actionInsert(uint32_t tick, uint8_t id);

#endif
