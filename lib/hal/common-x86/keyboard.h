#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	void keyboardInit(void);

	int keyboardGetChar(void);

	void keyboardSetLeds(bool num, bool caps, bool scroll);

	void keyboardEnable(bool enable);

#ifdef __cplusplus
}
#endif
