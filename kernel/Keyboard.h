#pragma once

#include "Queue.h"
#include "String.h"
#include <hal/cpu.h>

class Keyboard {

public:
	Keyboard();
	~Keyboard();

	int	GetChar();
	String GetString();
	void Scan(const String& fmt, ...);

	void UpdateLeds();

private:

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
		KEY_POUND             = 'Ј',
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

		KEY_UNKNOWN,
		KEY_NUMKEYCODES
	};

	// keyboard encoder ------------------------------------------

	enum EncoderIO {
		ENC_INPUT_BUF	=	0x60,
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
	void HandleInterrupt(RegisterFrame* regs);
	uint8_t ControlerGetStatus();
	void ControlerSendCommand(uint8_t cmd);
	uint8_t EncoderReadBuffer();
	void EncoderSendCommand(uint8_t cmd);
	void SetLeds(bool scroll, bool num, bool caps);

	void ResetSystem();
	bool SelfTest();
	void Enable(bool enabled = true);

	char KeyCodeToASCII (const KeyCode code);

private:
	static const int KeyReleased;

	static KeyCode mKeyScancodes[];
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
};
