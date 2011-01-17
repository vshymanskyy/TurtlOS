#pragma once
#include "CliPanel.h"
#include <hal/portio.h>

#define TEXT_INDEX		0x03D4
#define TEXT_DATA		0x03D5
#define TEXT_CURSOR_LO	0x0F
#define TEXT_CURSOR_HI	0x0E

class CliDesktop
	: public CliPanel
{

public:
	CliDesktop(const Rect& bounds)
		: CliPanel(bounds)
	{
		delete[] _canvas.Buffer();
		_canvas.Buffer((uint16_t*)0xB8000);
		Invalidate();
	}

	~CliDesktop() {
		_canvas.Buffer(NULL);
	}

	void SetCursor(const Vec2d& p) {
		const uint16_t pos = static_cast<uint16_t>(_canvas.Size().x * p.y + p.x);
		outportb(TEXT_INDEX, TEXT_CURSOR_LO);
		outportb(TEXT_DATA, static_cast<uint8_t>(pos));
		outportb(TEXT_INDEX, TEXT_CURSOR_HI);
		outportb(TEXT_DATA, static_cast<uint8_t>(pos >> 8));
	}

};
