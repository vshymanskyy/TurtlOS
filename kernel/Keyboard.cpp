#include "Keyboard.h"
#include "Console.h"

#include <ctype.h>
#include <stdlib.h>
#include <hal/portio.h>
#include <hal/cpu.h>
#include <hal/pic.h>

Keyboard::KeyCode Keyboard::mKeyScancodes[] = {
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

const int Keyboard::KeyReleased = 0x80;

int
Keyboard::GetChar()
{
	int result = 0;
	while(!result){
		while(mBuffer.IsEmpty()){
			cpuHalt();
		}
		result = KeyCodeToASCII(mBuffer.Pop());
	}
	return result;
}

void
Keyboard::HandleInterrupt(RegisterFrame* regs)
{
	if (ControlerGetStatus() & CTRL_STATS_MASK_OUT_BUF) {

		int scancode = EncoderReadBuffer ();

		if (scancode == 0xE0 || scancode == 0xE1){
			mExtended = true;
		} else {
			if (scancode & KeyReleased){		//KEY_UP
				scancode -= KeyReleased;
				const KeyCode key = mKeyScancodes[scancode];
				switch(key) {
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
						break;
				}
			}else{						//KEY_DOWN
				const KeyCode key = mKeyScancodes[scancode];
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
						UpdateLeds();
						break;
					case KEY_KP_NUMLOCK:
						mNumLock = !mNumLock;
						UpdateLeds();
						break;
					case KEY_SCROLLLOCK:
						mScrollLock = !mScrollLock;
						UpdateLeds();
						break;

					default:
						mBuffer.Push(key);
							//(*console) << "Keyboard buffer overflow." << endl;
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

char
Keyboard::KeyCodeToASCII (const KeyCode code)
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

	if (code >= 'a' && code <= 'z'){
		if(mShift ^ mCapsLock)
			return code - 32;
		else
			return code;
	}

	return isprint(code)? code : 0;
}

void
Keyboard::SetLeds(bool num, bool caps, bool scroll)
{
	uint8_t data = 0;
	if(scroll)
		data |= LED_SCROLL_LOCK;
	if(num)
		data |= LED_NUM_LOCK;
	if(caps)
		data |= LED_CAPS_LOCK;

	static uint8_t prev_temp = ~data;
	if(prev_temp != data){
		EncoderSendCommand (ENC_CMD_SET_LED);
		EncoderSendCommand (data);
		prev_temp = data;
	}
}

void
Keyboard::UpdateLeds()
{
	SetLeds(mNumLock, mCapsLock, mScrollLock);
}

String
Keyboard::GetString()
{
	char buff[1024];
	const int count = 1022;
	char* s = buff;
	*s = 0;
	for(;;){
		switch(char c = GetChar()){
			case '\n':
				//(*console).NewLine();
				return String(buff);
			case '\b':
				if(s > buff){
					*(--s) = 0;
					(*console) << '\b';
				}
				break;
			default:
				if((s - buff) <= count && isprint(c)){
					*(s++) = c;
					*s = 0;
					(*console) << c;
				}
				break;
		}
	}
	return String();
}

static
void
KeyboardIrqHandler(RegisterFrame* regs)
{
	debug_puts("KeyboardIRQ\n");
	picSendEOI(PIC_IRQ_KEYBOARD);
}

Keyboard::Keyboard()
{
	mCapsLock = mScrollLock = mNumLock = false;
	mCtrl = mAlt = mShift = false;
	mBat = mDiag = true;
	mResend = false;

	halCpuRegisterIRQ(PIC_IRQ_KEYBOARD, KeyboardIrqHandler);
	picClearMask(PIC_IRQ_KEYBOARD);

	UpdateLeds();
}

Keyboard::~Keyboard()
{
}

void
Keyboard::Scan(const String& fmt, ...)
{
	const char* format = fmt;
	char**  arg = (char**)&format+1;
	while (char c = *format++){
		if (c == '%'){
			c = *format++;
			const char* buff = GetString();
			switch (c){
				case 'd':
				case 'u':
				case 'l':
					**((long long**)arg++) = strtol(buff, NULL, 10);
					break;
// 				case 'x':
// 					atoll(buff, 16);
// 					break;
				case 's':
					strcpy(*arg++, buff);
					break;
				default:
					assert(false);
					break;
			}
		} else
			(*console) << c;
	}
}

void
Keyboard::EncoderSendCommand(uint8_t cmd)
{
	while (ControlerGetStatus () & CTRL_STATS_MASK_IN_BUF){
	}
	outb (ENC_CMD_REG, cmd);
}

uint8_t
Keyboard::EncoderReadBuffer()
{
	return inb (ENC_INPUT_BUF);
}

void
Keyboard::ControlerSendCommand(uint8_t cmd)
{
	while (ControlerGetStatus() & CTRL_STATS_MASK_IN_BUF) {
	}
	outb (CTRL_CMD_REG, cmd);
}

uint8_t
Keyboard::ControlerGetStatus()
{
	return inb (CTRL_STATS_REG);
}

void
Keyboard::ResetSystem()
{
	ControlerSendCommand (CTRL_CMD_WRITE_OUT_PORT);
	EncoderSendCommand (0xFE);
}

bool
Keyboard::SelfTest()
{
	ControlerSendCommand (CTRL_CMD_SELF_TEST);
	while (!(ControlerGetStatus() & CTRL_STATS_MASK_OUT_BUF)) {
	}
	return (EncoderReadBuffer() == 0x55);
}

void
Keyboard::Enable(bool enable)
{
	ControlerSendCommand((enable)?CTRL_CMD_ENABLE:CTRL_CMD_DISABLE);
	mEnabled = enable;
}
