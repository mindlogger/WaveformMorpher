#ifndef UI_H
#define UI_H
void setupUI();
unsigned int readADC(unsigned int pin);// pin:0-7;ret:0 - 4096
void setADSRValue(double value, uint8_t id);
void getADCValues();
void postUISem();
void *handle_ui(void *arg);
void *handle_input(void *arg);
#endif
