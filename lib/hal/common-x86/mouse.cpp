#include "mouse.h"
#include "portio.h"
#include "pic.h"
#include "cpu-common-x86.h"


#define MOUSE_CMD_RESET 0xFF 	 		// sends ACK (0xFA) and resets
#define MOUSE_CMD_RESEND 0xFE 	 		// sends its second last packet to the host again
#define MOUSE_CMD_SET_DEFAULTS 0xF6		// sends ACK (0xFA) and applies default values
#define MOUSE_CMD_DISABLE 0xF5 			// sends ACK (0xFA) and disable data reporting
#define MOUSE_CMD_ENABLE 0xF4 			// sends ACK (0xFA) and enables data reporting
#define MOUSE_CMD_SET_SAMPLE_RATE 0xF3	// sends ACK (0xFA) and waits for the next data, written to the mouse, to be the sample rate
#define MOUSE_CMD_GET_DEVICE_ID 0xF2	// sends ACK (0xFA) and sends its ID, though the DATA port
#define MOUSE_CMD_SET_REMOTE_MODE 0xF0	// sends ACK (0xFA) and then reset its movement counters, and enters remote mode
#define MOUSE_CMD_SET_WRAP_MODE 0xEE	// sends ACK (0xFA) and then reset its movement counters, and enters wrap mode
#define MOUSE_CMD_RESET_WRAP 0xEC		// sends ACK, and then enters the last mode, before entering wrap mode, it also resets its movement counters
#define MOUSE_CMD_READ_DATA 0xEB 		// sends ACK, followed by the first packet, and so on, until It have sent all
#define MOUSE_CMD_SET_STREAM_MODE 0xEA	// sends ACK (0xFA) and then reset its movement counters, and enters reporting mode
#define MOUSE_CMD_STATUS_REQ 0xE9 		// sends ACK, and sends afterwards the status uint8_ts ( 3 uint8_ts in a row, remember to wait each time for new data)
#define MOUSE_CMD_SET_RESOLUTION 0xE8 	// sends ACK and waits for the next data, written to the mouse, to be the resolution
#define MOUSE_CMD_SET_SCALING1 0xE6 	// sends ACK and set scaling 1:1
#define MOUSE_CMD_SET_SCALING2 0xE7 	// sends ACK and set scaling 2:1

#define Get_Compaq_Status_Byte 0x20
#define Set_Compaq_Status_Byte 0x60

#define MOUSE_WAIT_READ 0
#define MOUSE_WAIT_WRITE 1
#define MOUSE_WAIT_ACK_FA 2
#define MOUSE_WAIT_ACK_AA 3

#if 0

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

static
void
MouseIrqHandler(RegisterFrame* regs)
{
	if (inb(CTRL_STATS_REG) & CTRL_STATS_MASK_OUT_BUF)
	{
		mByte[mCycle++] = inb(ENC_INPUT_REG);


}

#endif

uint8_t mCycle;
uint8_t mCycleMax;
uint8_t mFlags;
uint8_t mByte[8];
int16_t mPosX, mPosY;
int16_t mVelX, mVelY;

//Mouse functions
static
void MouseIrqHandler(RegisterFrame *a_r)
{
	//debug_putc('|');
	if (inb(0x64) & 0x01)
	{
		mByte[mCycle++] = inb(0x60);
		if (mCycle >= mCycleMax)
		{
			mCycle = 0;
			uint8_t flags = mByte[0];
			int16_t dX = mByte[1] - ((flags << 4) & 0x100);
			int16_t dY = mByte[2] - ((flags << 3) & 0x100);
#if 0
			if (mCycleMax >= 4)
			{
				debug_print("[MOUSE] 4: %08b\n", mByte[3]);
			}
			if (dX || dY) {
				debug_print("[MOUSE] Delta: X %d, Y %d\n", dX, dY);
			}
			if (flags & 0x1 ^ mFlags & 0x1) {
				debug_print("[MOUSE] Left %s\n", (flags & 0x1) ? "down" : "up");
			}
			if (flags & 0x2 ^ mFlags & 0x2) {
				debug_print("[MOUSE] Right %s\n", (flags & 0x2) ? "down" : "up");
			}
			if (flags & 0x4 ^ mFlags & 0x4) {
				debug_print("[MOUSE] Middle %s\n", (flags & 0x4) ? "down" : "up");
			}
#endif
			mFlags = flags;

			int16_t posX = mPosX + dX;
			int16_t posY = mPosY - dY;
			if (posX<0) posX = 0;
			if (posY<0) posY = 0;
			if (posX>79*mVelX) posX = 79*mVelX;
			if (posY>24*mVelY) posY = 24*mVelY;
			if (posX != mPosX || posY != mPosY)
			{
				char* prevAttr = (char*)0xB8001 + (80*(mPosY/mVelY) + (mPosX/mVelX))*2;
				char* nextAttr = (char*)0xB8001 + (80* (posY/mVelY) +  (posX/mVelX))*2;
				*prevAttr = ~*prevAttr;
				*nextAttr = ~*nextAttr;
				mPosX = posX;
				mPosY = posY;
			}
#if 0
			*((char*)0xB8000 + 0) = dX;
			*((char*)0xB8000 + 2) = dY;
#endif
		}
	}
	picSendEOI(PIC_IRQ_MOUSE);
}

uint8_t mouse_read();

inline
bool mouse_wait(uint8_t option)
{
	volatile unsigned int time = 100;
	while (time--) {
		switch (option) {
		case MOUSE_WAIT_WRITE:
			if ((inb(0x64) & 2) == 0) return true;
			break;
		case MOUSE_WAIT_READ:
			if ((inb(0x64) & 1) == 1) return true;
			break;
		case MOUSE_WAIT_ACK_FA:
			if (mouse_read() == 0xFA) return true;
			break;
		case MOUSE_WAIT_ACK_AA:
			if (mouse_read() == 0xAA) return true;
			break;
		}
		for (volatile int i=0; i<1000; i++) { }
	}
	debug_print("[MOUSE] Wait timeout (%d)\n", option);
	return false;
}

uint8_t mouse_read()
{
	//Get's response from mouse
	mouse_wait(MOUSE_WAIT_READ);
	return inb(0x60);
}

inline
void mouse_write(uint8_t a_write)
{
	//debug_print("[MOUSE] Command %X\n", a_write);
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0xD4);
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x60, a_write);
}

inline
bool mouse_write_ack(uint8_t a_write)
{
	mouse_write(a_write);
	return mouse_wait(MOUSE_WAIT_ACK_FA);
}

void kb_reboot()
{
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0xFE);
}


enum MouseResolution {
	MouseRes1upmm = 0,
	MouseRes2upmm = 1,
	MouseRes4upmm = 2,
	MouseRes8upmm = 3
};

enum MouseType {
	MousePS2	= 0x00,
	MouseBallPoint	= 0x02,
	MouseIntellimouse	= 0x03,
	MouseExplorer	= 0x04,
	Mouse4d	= 0x06,
	Mouse4dPlus	= 0x08,
	MouseTyphoon	= 0x08
};


bool Reset() {
	return mouse_write_ack(MOUSE_CMD_RESET) &&
		   mouse_wait(MOUSE_WAIT_ACK_AA);
}

void SetResolution(MouseResolution res)
{
	mouse_write_ack(MOUSE_CMD_SET_RESOLUTION);
	mouse_write_ack((uint8_t)res);
}

void SetSampleRate(int rate)
{
	mouse_write_ack(MOUSE_CMD_SET_SAMPLE_RATE);
	mouse_write_ack(rate);
}

MouseType ReadMouseID()
{
	mouse_write_ack(MOUSE_CMD_GET_DEVICE_ID);
	return (MouseType)mouse_read();
}

MouseType DetectType()
{
	MouseType res;
/*
	SetSampleRate(200);
	SetSampleRate(100);
	SetSampleRate(80);
	SetSampleRate(60);
	SetSampleRate(40);
	SetSampleRate(20);
	return ReadMouseID() == MouseTyphoon;
	
*/

	SetSampleRate(200);
	SetSampleRate(100);
	SetSampleRate(80);
	res = ReadMouseID();
	if (res == MouseIntellimouse)
	{
		SetSampleRate(200);
		SetSampleRate(200);
		SetSampleRate(80);
		res = ReadMouseID();
	}
	return res;
}

void mouseInit()
{
	mCycle = 0;
	mCycleMax = 3;
	mPosX = 0;
	mPosY = 0;
	mVelX = 8;
	mVelY = mVelX*14/9;		// Compensate pixel shape
	
	char* prevAttr = (char*)0xB8001;
	*prevAttr = ~*prevAttr;

	cpuDisableInterrupts();
	uint8_t data;

	// Disable first PS/2 port
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0xAD);

	// Disable second PS/2 port
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0xA7);

	// Disable interrupts
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0x20);
	mouse_wait(MOUSE_WAIT_READ);
	data = inb(0x60);
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0x60);
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x60, data & ~0x03); //~0x40

	// Test
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0xAA);
	mouse_wait(0);
	if (0x55 != inb(0x60))
	{
		debug_print("[MOUSE] PS/2 controller test failed\n");
		cpuEnableInterrupts();
		return;
	}

	// Enable first PS/2 port
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0xAE);

	// Enable second PS/2 port
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0xA8);


	// Enable interrupts
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0x20);
	mouse_wait(MOUSE_WAIT_READ);
	data = inb(0x60);
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x64, 0x60);
	mouse_wait(MOUSE_WAIT_WRITE);
	outb(0x60, data | 0x03);


/*
	if (!mouse_write_ack(MOUSE_CMD_SET_DEFAULTS)) {
		debug_print("[MOUSE] Could not set mouse to default\n");
		cpuEnableInterrupts();
		return;
	}
*/
	Reset();
	switch (DetectType()) {
	case MouseTyphoon:
		mCycleMax = 6;
		debug_print("[MOUSE] Mode: Typhoon\n");
		break;
	case MouseExplorer:
		mCycleMax = 4;
		debug_print("[MOUSE] Mode: Explorer\n");
		break;
	case MouseIntellimouse:
		mCycleMax = 4;
		debug_print("[MOUSE] Mode: Intellimouse\n");
		break;
	default:
		debug_print("[MOUSE] Mode: Other\n");
		break; 
	}
	
	if (!mouse_write_ack(MOUSE_CMD_SET_SCALING1)) {
		debug_print("[MOUSE] Could not set scaling\n");
		cpuEnableInterrupts();
		return;
	}
	
	SetResolution(MouseRes1upmm);
	
	if (!mouse_write_ack(MOUSE_CMD_ENABLE)) {
		debug_print("[MOUSE] Could not enable mouse\n");
		cpuEnableInterrupts();
		return;
	}

	halCpuRegisterIRQ(PIC_IRQ_MOUSE, MouseIrqHandler);
	picSetMask(PIC_IRQ_MOUSE, false);

	//kb_reboot();

	cpuEnableInterrupts();
}
 
