
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
#define MOUSE_CMD_SET_WRAP_MODE 0xEE	// sends ACK (0xFA) and then reset its movement counters, and enters warp mode
#define MOUSE_CMD_RESET_WRAP 0xEC		// sends ACK, and then enters the last mode, before entering wrap mode, it also resets its movement counters
#define MOUSE_CMD_READ_DATA 0xEB 		// sends ACK, followed by the first packet, and so on, until It have sent all
#define MOUSE_CMD_SET_STREAM_MODE 0xEA	// sends ACK (0xFA) and then reset its movement counters, and enters reporting mode
#define MOUSE_CMD_STATUS_REQ 0xE9 		// sends ACK, and sends afterwards the status bytes ( 3 bytes in a row, remember to wait each time for new data)
#define MOUSE_CMD_SET_RESOLUTION 0xE8 	// sends ACK and waits for the next data, written to the mouse, to be the resolution
#define MOUSE_CMD_SET_SCALING1 0xE6 	// sends ACK and set scaling 1:1
#define MOUSE_CMD_SET_SCALING2 0xE7 	// sends ACK and set scaling 2:1

#define Get_Compaq_Status_Byte 0x20
#define Set_Compaq_Status_Byte 0x60

#define MOUSE_WAIT_READ 0
#define MOUSE_WAIT_WRITE 1
#define MOUSE_WAIT_ACK 2
#define MOUSE_WAIT_ACK_AA 3
#define MOUSE_WAIT_READY 4

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
mouseWait(uint8_t option)
{
	unsigned int time = 10000;
	while (time--) {
		switch (option) {
		case MOUSE_WAIT_WRITE:
			if ((inb(0x64) & 2) == 0) return;
			break;
		case MOUSE_WAIT_READ:
			if ((inb(0x64) & 1) == 1) return;
			break;
		case MOUSE_WAIT_ACK:
			if (inb(0x64) == 0xFA) return;
			break;
		case MOUSE_WAIT_ACK_AA:
			if (inb(0x64) == 0xAA) return;
			break;
		case MOUSE_WAIT_READY:
			if ((inb(0x64) & 0x21) == 0x21) return;
			break;
		}
	}
	debug_print("[MOUSE]\tWait timeout (%d)\n", option);
}

static
void
mouseWrite(uint8_t cmd)
{
	mouseWait(MOUSE_WAIT_WRITE);
	outb(0x64, 0xD4);
	mouseWait(MOUSE_WAIT_WRITE);
	outb(0x60, cmd);
	mouseWait(MOUSE_WAIT_ACK);
}

uint8_t mCycle;
int8_t mByte[3];
int16_t mPositionX;
int16_t mPositionY;

void
MouseIrqHandler(RegisterFrame* regs)
{
	assert("hooray");
	if (inb(CTRL_STATS_REG) & CTRL_STATS_MASK_OUT_BUF) {
		mByte[mCycle++] = inb(ENC_INPUT_REG);

		if(mCycle == 3){
			mCycle=0;
			mPositionX += mByte[1];
			mPositionY += mByte[2];

			uint16_t* disp = (uint16_t*)0xB8000;
			disp[0] = mPositionX;
			disp[1] = mPositionY;
		}
	}

	picSendEOI(PIC_IRQ_MOUSE);
}


void
mouseInit(void)
{
	cpuDisableInterrupts();

    mouseWait(MOUSE_WAIT_WRITE);
    outb(0x64, 0xA8);
    mouseWait(MOUSE_WAIT_ACK);
    mouseWait(MOUSE_WAIT_WRITE);
    outb(0x64, 0x20);
    mouseWait(MOUSE_WAIT_ACK);
    mouseWait(MOUSE_WAIT_READ);
    uint8_t status = (inb(0x60) | 2);
    mouseWait(MOUSE_WAIT_WRITE);
    outb(0x64, 0x60);
    mouseWait(MOUSE_WAIT_ACK);
    mouseWait(MOUSE_WAIT_WRITE);
    outb(0x60, status);
    mouseWrite(0xF6);
    mouseWrite(0xF4);

	//Setup the mouse handler
	halCpuRegisterIRQ(PIC_IRQ_MOUSE, MouseIrqHandler);
	picClearMask(PIC_IRQ_MOUSE);

	cpuEnableInterrupts();
}
