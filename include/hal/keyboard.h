#pragma once

void keyboardInit(void);

int keyboardGetChar(void);

void keyboardSetLeds(bool num, bool caps, bool scroll);

void keyboardEnable(bool enable);
