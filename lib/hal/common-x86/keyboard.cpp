#include "keyboard.h"
#include "cpu-common-x86.h"
#include "pic.h"
#include "portio.h"
#include <ctype.h>

enum KeyCode {
	// Alphanumeric keys ////////////////

	KEY_SPACE             = ' ',
	KEY_0                 = '0',
	KEY_1                 = '1',
	KEY_2                 = '2',
	KEY_3                 = '3',
	KEY_4                 = '4',
	KEY_5                 = '5',
	KEY_6                 = '6',
	KEY_7                 = '7',
	KEY_8                 = '8',
	KEY_9                 = '9',

	KEY_A                 = 'a',
	KEY_B                 = 'b',
	KEY_C                 = 'c',
	KEY_D                 = 'd',
	KEY_E                 = 'e',
	KEY_F                 = 'f',
	KEY_G                 = 'g',
	KEY_H                 = 'h',
	KEY_I                 = 'i',
	KEY_J                 = 'j',
	KEY_K                 = 'k',
	KEY_L                 = 'l',
	KEY_M                 = 'm',
	KEY_N                 = 'n',
	KEY_O                 = 'o',
	KEY_P                 = 'p',
	KEY_Q                 = 'q',
	KEY_R                 = 'r',
	KEY_S                 = 's',
	KEY_T                 = 't',
	KEY_U                 = 'u',
	KEY_V                 = 'v',
	KEY_W                 = 'w',
	KEY_X                 = 'x',
	KEY_Y                 = 'y',
	KEY_Z                 = 'z',

	KEY_RETURN            = '\r',
	KEY_ESCAPE            = 0x1001,
	KEY_BACKSPACE         = '\b',

	// Arrow keys ////////////////////////

	KEY_UP                = 0x1100,
	KEY_DOWN              = 0x1101,
	KEY_LEFT              = 0x1102,
	KEY_RIGHT             = 0x1103,

	// Function keys /////////////////////

	KEY_F1                = 0x1201,
	KEY_F2                = 0x1202,
	KEY_F3                = 0x1203,
	KEY_F4                = 0x1204,
	KEY_F5                = 0x1205,
	KEY_F6                = 0x1206,
	KEY_F7                = 0x1207,
	KEY_F8                = 0x1208,
	KEY_F9                = 0x1209,
	KEY_F10               = 0x120a,
	KEY_F11               = 0x120b,
	KEY_F12               = 0x120b,
	KEY_F13               = 0x120c,
	KEY_F14               = 0x120d,
	KEY_F15               = 0x120e,

	KEY_DOT               = '.',
	KEY_COMMA             = ',',
	KEY_COLON             = ':',
	KEY_SEMICOLON         = ';',
	KEY_SLASH             = '/',
	KEY_BACKSLASH         = '\\',
	KEY_PLUS              = '+',
	KEY_MINUS             = '-',
	KEY_ASTERISK          = '*',
	KEY_EXCLAMATION       = '!',
	KEY_QUESTION          = '?',
	KEY_QUOTEDOUBLE       = '\"',
	KEY_QUOTE             = '\'',
	KEY_EQUAL             = '=',
	KEY_HASH              = '#',
	KEY_PERCENT           = '%',
	KEY_AMPERSAND         = '&',
	KEY_UNDERSCORE        = '_',
	KEY_LEFTPARENTHESIS   = '(',
	KEY_RIGHTPARENTHESIS  = ')',
	KEY_LEFTBRACKET       = '[',
	KEY_RIGHTBRACKET      = ']',
	KEY_LEFTCURL          = '{',
	KEY_RIGHTCURL         = '}',
	KEY_DOLLAR            = '$',
	KEY_POUND             = 'J',
	KEY_EURO              = '$',
	KEY_LESS              = '<',
	KEY_GREATER           = '>',
	KEY_BAR               = '|',
	KEY_GRAVE             = '`',
	KEY_TILDE             = '~',
	KEY_AT                = '@',
	KEY_CARRET            = '^',

	// Numeric keypad //////////////////////

	KEY_KP_0              = '0',
	KEY_KP_1              = '1',
	KEY_KP_2              = '2',
	KEY_KP_3              = '3',
	KEY_KP_4              = '4',
	KEY_KP_5              = '5',
	KEY_KP_6              = '6',
	KEY_KP_7              = '7',
	KEY_KP_8              = '8',
	KEY_KP_9              = '9',
	KEY_KP_PLUS           = '+',
	KEY_KP_MINUS          = '-',
	KEY_KP_DECIMAL        = '.',
	KEY_KP_DIVIDE         = '/',
	KEY_KP_ASTERISK       = '*',
	KEY_KP_NUMLOCK        = 0x300f,
	KEY_KP_ENTER          = 0x3010,

	KEY_TAB               = 0x4000,
	KEY_CAPSLOCK          = 0x4001,

	// Modify keys ////////////////////////////

	KEY_LSHIFT            = 0x4002,
	KEY_LCTRL             = 0x4003,
	KEY_LALT              = 0x4004,
	KEY_LWIN              = 0x4005,
	KEY_RSHIFT            = 0x4006,
	KEY_RCTRL             = 0x4007,
	KEY_RALT              = 0x4008,
	KEY_RWIN              = 0x4009,

	KEY_INSERT            = 0x400a,
	KEY_DELETE            = 0x400b,
	KEY_HOME              = 0x400c,
	KEY_END               = 0x400d,
	KEY_PAGEUP            = 0x400e,
	KEY_PAGEDOWN          = 0x400f,
	KEY_SCROLLLOCK        = 0x4010,
	KEY_PAUSE             = 0x4011,

	KEY_UNKNOWN			  = 0xFFFF
};

// keyboard encoder ------------------------------------------

enum EncoderIO {
	ENC_INPUT_REG	=	0x60,
	ENC_CMD_REG		=	0x60
};

// keyboard controller ---------------------------------------

enum ControllerIO {
	CTRL_STATS_REG	=	0x64,
	CTRL_CMD_REG	=	0x64
};

enum ControllerStatusMask {
	CTRL_STATS_MASK_OUT_BUF		=	1,		//00000001
	CTRL_STATS_MASK_IN_BUF		=	2,		//00000010
	CTRL_STATS_MASK_SYSTEM		=	4,		//00000100
	CTRL_STATS_MASK_CMD_DATA	=	8,		//00001000
	CTRL_STATS_MASK_LOCKED		=	0x10,	//00010000
	CTRL_STATS_MASK_AUX_BUF		=	0x20,	//00100000
	CTRL_STATS_MASK_TIMEOUT		=	0x40,	//01000000
	CTRL_STATS_MASK_PARITY		=	0x80	//10000000
};

enum EncoderCommands {
	ENC_CMD_SET_LED				=	0xED,
	ENC_CMD_ECHO				=	0xEE,
	ENC_CMD_SCAN_CODE_SET		=	0xF0,
	ENC_CMD_ID					=	0xF2,
	ENC_CMD_AUTODELAY			=	0xF3,
	ENC_CMD_ENABLE				=	0xF4,
	ENC_CMD_RESETWAIT			=	0xF5,
	ENC_CMD_RESETSCAN			=	0xF6,
	ENC_CMD_ALL_AUTO			=	0xF7,
	ENC_CMD_ALL_MAKEBREAK		=	0xF8,
	ENC_CMD_ALL_MAKEONLY		=	0xF9,
	ENC_CMD_ALL_MAKEBREAK_AUTO	=	0xFA,
	ENC_CMD_SINGLE_AUTOREPEAT	=	0xFB,
	ENC_CMD_SINGLE_MAKEBREAK	=	0xFC,
	ENC_CMD_SINGLE_BREAKONLY	=	0xFD,
	ENC_CMD_RESEND				=	0xFE,
	ENC_CMD_RESET				=	0xFF
};

enum ControllerCommands {
	CTRL_CMD_READ				=	0x20,
	CTRL_CMD_WRITE				=	0x60,
	CTRL_CMD_SELF_TEST			=	0xAA,
	CTRL_CMD_INTERFACE_TEST		=	0xAB,
	CTRL_CMD_DISABLE			=	0xAD,
	CTRL_CMD_ENABLE				=	0xAE,
	CTRL_CMD_READ_IN_PORT		=	0xC0,
	CTRL_CMD_READ_OUT_PORT		=	0xD0,
	CTRL_CMD_WRITE_OUT_PORT		=	0xD1,
	CTRL_CMD_READ_TEST_INPUTS	=	0xE0,
	CTRL_CMD_SYSTEM_RESET		=	0xFE,
	CTRL_CMD_MOUSE_DISABLE		=	0xA7,
	CTRL_CMD_MOUSE_ENABLE		=	0xA8,
	CTRL_CMD_MOUSE_PORT_TEST	=	0xA9,
	CTRL_CMD_MOUSE_WRITE		=	0xD4
};

enum LedMask {
	LED_SCROLL_LOCK			= 1,
	LED_NUM_LOCK			= 2,
	LED_CAPS_LOCK			= 4
};

// scan error codes ------------------------------------------

enum Errors {
	ERR_BUF_OVERRUN			=	0,
	ERR_ID_RET				=	0x83AB,
	ERR_BAT					=	0xAA,	//note: can also be L. shift key make code
	ERR_ECHO_RET			=	0xEE,
	ERR_ACK					=	0xFA,
	ERR_BAT_FAILED			=	0xFC,
	ERR_DIAG_FAILED			=	0xFD,
	ERR_RESEND_CMD			=	0xFE,
	ERR_KEY					=	0xFF
};

static
const int KeyReleased = 0x80;

static
KeyCode KeyScancodes[] = {
	// key				scancode
	KEY_UNKNOWN,		//0x00
	KEY_ESCAPE,			//0x01
	KEY_1,				//0x02
	KEY_2,				//0x03
	KEY_3,				//0x04
	KEY_4,				//0x05
	KEY_5,				//0x06
	KEY_6,				//0x07
	KEY_7,				//0x08
	KEY_8,				//0x09
	KEY_9,				//0x0a
	KEY_0,				//0x0b
	KEY_MINUS,			//0x0c
	KEY_EQUAL,			//0x0d
	KEY_BACKSPACE,		//0x0e
	KEY_TAB,			//0x0f
	KEY_Q,				//0x10
	KEY_W,				//0x11
	KEY_E,				//0x12
	KEY_R,				//0x13
	KEY_T,				//0x14
	KEY_Y,				//0x15
	KEY_U,				//0x16
	KEY_I,				//0x17
	KEY_O,				//0x18
	KEY_P,				//0x19
	KEY_LEFTBRACKET,	//0x1a
	KEY_RIGHTBRACKET,	//0x1b
	KEY_RETURN,			//0x1c
	KEY_LCTRL,			//0x1d
	KEY_A,				//0x1e
	KEY_S,				//0x1f
	KEY_D,				//0x20
	KEY_F,				//0x21
	KEY_G,				//0x22
	KEY_H,				//0x23
	KEY_J,				//0x24
	KEY_K,				//0x25
	KEY_L,				//0x26
	KEY_SEMICOLON,		//0x27
	KEY_QUOTE,			//0x28
	KEY_GRAVE,			//0x29
	KEY_LSHIFT,			//0x2a
	KEY_BACKSLASH,		//0x2b
	KEY_Z,				//0x2c
	KEY_X,				//0x2d
	KEY_C,				//0x2e
	KEY_V,				//0x2f
	KEY_B,				//0x30
	KEY_N,				//0x31
	KEY_M,				//0x32
	KEY_COMMA,			//0x33
	KEY_DOT,			//0x34
	KEY_SLASH,			//0x35
	KEY_RSHIFT,			//0x36
	KEY_KP_ASTERISK,	//0x37
	KEY_RALT,			//0x38
	KEY_SPACE,			//0x39
	KEY_CAPSLOCK,		//0x3a
	KEY_F1,				//0x3b
	KEY_F2,				//0x3c
	KEY_F3,				//0x3d
	KEY_F4,				//0x3e
	KEY_F5,				//0x3f
	KEY_F6,				//0x40
	KEY_F7,				//0x41
	KEY_F8,				//0x42
	KEY_F9,				//0x43
	KEY_F10,			//0x44
	KEY_KP_NUMLOCK,		//0x45
	KEY_SCROLLLOCK,		//0x46
	KEY_HOME,			//0x47
	KEY_KP_8,			//0x48	//keypad up arrow
	KEY_PAGEUP,			//0x49
	KEY_KP_2,			//0x50	//keypad down arrow
	KEY_KP_3,			//0x51	//keypad page down
	KEY_KP_0,			//0x52	//keypad insert key
	KEY_KP_DECIMAL,		//0x53	//keypad delete key
	KEY_UNKNOWN,		//0x54
	KEY_UNKNOWN,		//0x55
	KEY_UNKNOWN,		//0x56
	KEY_F11,			//0x57
	KEY_F12				//0x58
};

#include <std/Queue.h>

bool mShift;
bool mCtrl;
bool mAlt;
bool mCapsLock;
bool mNumLock;
bool mScrollLock;
bool mExtended;
bool mEnabled;
bool mResend;
bool mBat;
bool mDiag;
Queue<KeyCode, 128> mBuffer;

static char
KeyCodeToASCII (const KeyCode code)
{
	switch (code) {
		case KEY_BACKSPACE: return '\b';
		case KEY_RETURN:	return '\n';
		case KEY_TAB:		return '\t';
		default:			break;
	}

	if (mShift){
		switch (code) {
			case KEY_1:				return KEY_EXCLAMATION;
			case KEY_2:				return KEY_AT;
			case KEY_3:				return KEY_HASH;
			case KEY_4:				return KEY_DOLLAR;
			case KEY_5:				return KEY_PERCENT;
			case KEY_6:				return KEY_CARRET;
			case KEY_7:				return KEY_AMPERSAND;
			case KEY_8:				return KEY_ASTERISK;
			case KEY_9:				return KEY_LEFTPARENTHESIS;
			case KEY_0:				return KEY_RIGHTPARENTHESIS;
			case KEY_COMMA:			return KEY_LESS;
			case KEY_DOT:			return KEY_GREATER;
			case KEY_SLASH:			return KEY_QUESTION;
			case KEY_SEMICOLON:		return KEY_COLON;
			case KEY_QUOTE:			return KEY_QUOTEDOUBLE;
			case KEY_LEFTBRACKET:	return KEY_LEFTCURL;
			case KEY_RIGHTBRACKET:	return KEY_RIGHTCURL;
			case KEY_GRAVE:			return KEY_TILDE;
			case KEY_MINUS:			return KEY_UNDERSCORE;
			case KEY_EQUAL:			return KEY_PLUS;
			case KEY_BACKSLASH:		return KEY_BAR;
			default:				break;
		}
	}

	if (mNumLock){
		switch (code) {
			case KEY_1:				return KEY_EXCLAMATION;
			case KEY_2:				return KEY_AT;
			case KEY_3:				return KEY_HASH;
			case KEY_4:				return KEY_DOLLAR;
			case KEY_5:				return KEY_PERCENT;
			case KEY_6:				return KEY_CARRET;
			case KEY_7:				return KEY_AMPERSAND;
			case KEY_8:				return KEY_ASTERISK;
			case KEY_9:				return KEY_LEFTPARENTHESIS;
			case KEY_0:				return KEY_RIGHTPARENTHESIS;
			case KEY_COMMA:			return KEY_LESS;
			case KEY_DOT:			return KEY_GREATER;
			case KEY_SLASH:			return KEY_QUESTION;
			case KEY_SEMICOLON:		return KEY_COLON;
			case KEY_QUOTE:			return KEY_QUOTEDOUBLE;
			case KEY_LEFTBRACKET:	return KEY_LEFTCURL;
			case KEY_RIGHTBRACKET:	return KEY_RIGHTCURL;
			case KEY_GRAVE:			return KEY_TILDE;
			case KEY_MINUS:			return KEY_UNDERSCORE;
			case KEY_EQUAL:			return KEY_PLUS;
			case KEY_BACKSLASH:		return KEY_BAR;
			default:				break;
		}
	}

	if (code >= 'a' && code <= 'z') {
		return (mShift ^ mCapsLock)?(code - 32):(code);
	}

	return isprint(code)? code : 0;
}

static
void
EncoderSendCommand(uint8_t cmd)
{
	while (inb(CTRL_STATS_REG) & CTRL_STATS_MASK_IN_BUF) {
	}
	outb (ENC_CMD_REG, cmd);
}


static
void
ControlerSendCommand(uint8_t cmd)
{
	while (inb(CTRL_STATS_REG) & CTRL_STATS_MASK_IN_BUF) {
	}
	outb (CTRL_CMD_REG, cmd);
}

void
keyboardEnable(bool enable)
{
	ControlerSendCommand((enable)?CTRL_CMD_ENABLE:CTRL_CMD_DISABLE);
	mEnabled = enable;
}


void
keyboardSetLeds(bool num, bool caps, bool scroll)
{
	EncoderSendCommand (ENC_CMD_SET_LED);
	EncoderSendCommand ((num?LED_NUM_LOCK:0) | (caps?LED_CAPS_LOCK:0) | (scroll?LED_SCROLL_LOCK:0));
}

static
void
KeyboardIrqHandler(RegisterFrame* regs)
{
	if (inb(CTRL_STATS_REG) & CTRL_STATS_MASK_OUT_BUF) {
		int scancode = inb(ENC_INPUT_REG);

		//int temp = inb(0x61);
		//outb(0x61, temp | 0x80);  /* Disable */
		//outb(0x61, temp & 0x7F);  /* Re-enable */

		if (scancode == 0xE0 || scancode == 0xE1){
			mExtended = true;
		} else {
			if (scancode & KeyReleased) {		//KEY_UP
				scancode &= ~KeyReleased;
				const KeyCode key = KeyScancodes[scancode];
				switch (key) {
					case KEY_LCTRL:
					case KEY_RCTRL:
						mCtrl = false;
						break;
					case KEY_LSHIFT:
					case KEY_RSHIFT:
						mShift = false;
						break;
					case KEY_LALT:
					case KEY_RALT:
						mAlt = false;
						break;
					default:
						debug_print("[KBD] Up '%c'\n", key);
						break;
				}
			} else {					//KEY_DOWN
				const KeyCode key = KeyScancodes[scancode];

				if (mCtrl && mAlt && (key == KEY_BACKSPACE)) {
					debug_print("[KBD] Reset system\n");
					ControlerSendCommand (CTRL_CMD_SYSTEM_RESET);
				}

				switch(key) {
					case KEY_LCTRL:
					case KEY_RCTRL:
						mCtrl = true;
						break;

					case KEY_LSHIFT:
					case KEY_RSHIFT:
						mShift = true;
						break;

					case KEY_LALT:
					case KEY_RALT:
						mAlt = true;
						break;

					case KEY_CAPSLOCK:
						mCapsLock = !mCapsLock;
						keyboardSetLeds(mNumLock, mCapsLock, mScrollLock);
						break;
					case KEY_KP_NUMLOCK:
						mNumLock = !mNumLock;
						keyboardSetLeds(mNumLock, mCapsLock, mScrollLock);
						break;
					case KEY_SCROLLLOCK:
						mScrollLock = !mScrollLock;
						keyboardSetLeds(mNumLock, mCapsLock, mScrollLock);
						break;

					default:
						debug_print("[KBD] Down '%c'\n", key);
						mBuffer.Push(key);
						break;
				}
			}

			mExtended = false;
		}

		//! watch for errors
		switch (scancode) {

			case ERR_BAT_FAILED:
				mBat = false;
				break;

			case ERR_DIAG_FAILED:
				mDiag = false;
				break;

			case ERR_RESEND_CMD:
				mResend = true;
				break;
		}
	}

	picSendEOI(PIC_IRQ_KEYBOARD);
}

void
keyboardInit(void)
{
	cpuDisableInterrupts();

	// Self-test
	ControlerSendCommand (CTRL_CMD_SELF_TEST);
	while (!(inb(CTRL_STATS_REG) & CTRL_STATS_MASK_OUT_BUF)) {
	}
	if (inb(ENC_INPUT_REG) != 0x55) {
		debug_print("[KBD] Self-test failed");
		cpuEnableInterrupts();
		return;
	}

	halCpuRegisterIRQ(PIC_IRQ_KEYBOARD, KeyboardIrqHandler);
	picClearMask(PIC_IRQ_KEYBOARD);

	cpuEnableInterrupts();
}

int
keyboardGetChar(void)
{
	int result = 0;
	while(!result){
		while(mBuffer.IsEmpty()){
			//cpuHalt();
		}
		result = KeyCodeToASCII(mBuffer.Pop());
	}
	return result;
}
