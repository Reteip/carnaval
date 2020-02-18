
#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

void InitLedDriver();

void SetStripOff();

void RainbowProgram(uint8_t index);

void Show_Cycle(uint8_t cycle);

void DoDelay(unsigned long ms);

void Show();

#endif