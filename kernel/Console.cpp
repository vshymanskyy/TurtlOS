#include "Console.h"

#include <hal/portio.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const char endl = '\n';
const char esc = (char)0x1B;

static const char* symb = "0123456789abcdefghijklmnopqrstuvwxyz";

Vec2d
Console::GetCursorPos()
{
	return _canvas.OffsetLocation(_cursor);
}

void
Console::UpdateCursor()
{
	SetCursor(GetCursorPos());
}


Console::Console(const Rect& bounds)
	: CliWindow(bounds)
	, _cursor(_canvas.Buffer())
	, _tab(4)
{
	ResetAttributes();
	Erase(BOTH, false);
}

void
Console::ResetAttributes()
{
	SetBold(false);
	SetBlink(false);
	SetReverseVideo(false);
	Background(BLACK);
	Foreground(WHITE);
}

void
Console::SetLineWrap(bool enabled)
{
	_lineWrap = enabled;
}

void
Console::SetReverseVideo (bool enabled)
{
	_reverseVideo = enabled;
}

void
Console::SetBlink(bool enabled)
{
	if (enabled) {
		//mAttr = static_cast<uint8_t>(static_cast<unsigned>(mAttr) | 0x08);
	} else {
		//mAttr = static_cast<uint8_t>(static_cast<unsigned>(mAttr) & 0xF7);
	}
}

void
Console::SetBold(bool enabled)
{
	if (enabled) {
		//mAttr = static_cast<uint8_t>(static_cast<unsigned>(mAttr) | 0x80);
	} else {
		//mAttr = static_cast<uint8_t>(static_cast<unsigned>(mAttr) & 0x7F);
	}
}


// Erases the screen from the current line down to the bottom of the screen.
// Erases the screen from the current line up to the top of the screen.
// Erases the screen with the background color and moves the cursor to home.

// Erases from the current cursor position to the end of the current line
// Erases from the current cursor position to the start of the current line
// Erases the entire current line

/// @todo Implement
void
Console::Erase(EraseDirection d, bool line)
{
	const uint16_t attr = GetAttr(0);
	if (line) {
		switch(d) {
			case FORWARD:
				break;
			case BACKWARD:
				break;
			case BOTH:
				break;
		}
	} else {
		switch(d) {
			case FORWARD:
				break;
			case BACKWARD:
				break;
			case BOTH: {
				_canvas.Fill(attr);
				_cursor = _canvas.Buffer();
				UpdateCursor();

				Invalidate();
				break;
			}
		}
	}
}

static
int
AnsiEscParseParams(char* s, long buff[])
{
	if (*s == '\0')
		return 0;

	int i = 0;
	for(;;) {
		buff[i++] = strtol(s, &s, 0);
		if (*s++ == '\0')
			break;
	}
	return i;
}

/// @todo Check boundaries
bool
Console::AnsiEscFilter(const char c)
{
	if (_ansiBufferPos == 0 && c == (char)0x9B) {
		_ansiBufferPos = 2;
		return true;
	}else if (_ansiBufferPos == 0 && c == (char)0x1B) {
		_ansiBufferPos = 1;
		return true;
	}else if (_ansiBufferPos == 1) {
		if (c == '[') {
			_ansiBufferPos = 2;
			return true;
		} else {
			_ansiBufferPos = 0;
			return false;
		}
	}else if (_ansiBufferPos > 1) {
		if (isdigit(c) || c == ';') {
			_ansiBuffer[_ansiBufferPos++] = c;
		} else {
			_ansiBuffer[_ansiBufferPos] = '\0';
			_ansiBufferPos = 0;

			long params[10] = {0};
			const int parQty = AnsiEscParseParams(&_ansiBuffer[2], params);
			switch(c) {
				case 'c':	// Reset Device
					ResetAttributes();
					Erase(BOTH, false);
					break;
				case 'h':
					if (parQty == 1 && params[0] == 7)
						SetLineWrap(true);
					break;
				case 'l':
					if (parQty == 1 && params[0] == 7)
						SetLineWrap(false);
					break;
				case 'A':	// [nA Cursor Up
					if (!parQty)
						params[0] = 1;
					//
					_cursor -= _canvas.Size().x*params[0];
					UpdateCursor();
					break;
				case 'B':	// [nB Cursor Down
					if (!parQty)
						params[0] = 1;
					//
					_cursor += _canvas.Size().x*params[0];
					UpdateCursor();
					break;
				case 'C':	// [nC Cursor Forward
					if (!parQty)
						params[0] = 1;
					//
					_cursor += params[0];
					UpdateCursor();
					break;
				case 'D':	// [nD Cursor Backward
					if (!parQty)
						params[0] = 1;
					//
					_cursor -= params[0];
					UpdateCursor();
					break;
				case 'H':	// [y;xH Cursor Home
				case 'f':	// [y;xf Force Cursor Position
					if (parQty < 2) {
						params[0] = 0;	// Y
						params[1] = 0;	// X
					}
					//
					_cursor = _canvas.Buffer() + _canvas.Size().x*params[0] + params[1];
					UpdateCursor();
					break;
				case 's':	// [s Save Cursor
					_ansiCursorStack.Push(_cursor);
					break;
				case 'u':	// [u Unsave Cursor
					_cursor = _ansiCursorStack.Pop();
					UpdateCursor();
					break;
				case 'J':	// [nJ
					if (parQty == 0)
						params[0] = 0;
					Erase(static_cast<EraseDirection>(params[0]), false);
					break;
				case 'K':	// [nK
					if (parQty == 0)
						params[0] = 0;
					Erase(static_cast<EraseDirection>(params[0]), true);
					break;
				case 'm':	// [p1;..;pNm
					if (!parQty) {
						ResetAttributes();
					} else {
						for(int i=0; i<parQty; ++i) {
							switch(params[i]) {
								case  0:	ResetAttributes();		break;
								case  1:	SetBold(true);			break;
								//case  2:	SetFaint(true);			break;
								//case  3:	SetItalic(true);		break;
								//case  4:	SetUnderline(true);		break;
								case  5:	SetBlink(true);			break;
								//case  6:	SetRapidBlink(true);	break;
								case  7:	SetReverseVideo(true);	break;
								//case  8:	SetInvisible(true);		break;
								//case 48:	SetSubscript(true);		break;
								//case 49:	SetSuperscript(true);	break;

								case 30:	Foreground(BLACK);	break;
								case 31:	Foreground(RED);	break;
								case 32:	Foreground(GREEN);	break;
								case 33:	Foreground(BROWN);	break;
								case 34:	Foreground(BLUE);	break;
								case 35:	Foreground(MAGENTA);break;
								case 36:	Foreground(CYAN);	break;
								case 37:	Foreground(WHITE);	break;
								case 90:	Foreground(GRAY);	break;
								case 91:	Foreground(RED_B);	break;
								case 92:	Foreground(LIME);	break;
								case 93:	Foreground(YELLOW);	break;
								case 94:	Foreground(BLUE_B);	break;
								case 95:	Foreground(MAGENTA_B);break;
								case 96:	Foreground(CYAN_B);	break;
								case 97:	Foreground(WHITE_B);break;

								case 40:	Background(BLACK);	break;
								case 41:	Background(RED);	break;
								case 42:	Background(GREEN);	break;
								case 43:	Background(BROWN);	break;
								case 44:	Background(BLUE);	break;
								case 45:	Background(MAGENTA);break;
								case 46:	Background(CYAN);	break;
								case 47:	Background(WHITE);	break;
								case 100:	Background(GRAY);	break;
								case 101:	Background(RED_B);	break;
								case 102:	Background(LIME);	break;
								case 103:	Background(YELLOW);	break;
								case 104:	Background(BLUE_B);	break;
								case 105:	Background(MAGENTA_B);break;
								case 106:	Background(CYAN_B);	break;
								case 107:	Background(WHITE_B);break;
							}
						}
					}
					break;
				default:		//the command was not recognized
					char* p = _ansiBuffer;
					while(*p)
						PutChar(*p++);
					break;
			}
		}
		return true;
	}
	return false;
}

void
Console::PutNewLine()
{
	if (_cursor > _canvas.BufferEnd()-_canvas.Size().x) {
		Scroll();
	}

	const int count = _canvas.Size().x - _canvas.GetOffset(_cursor)%_canvas.Size().x;

	memsetw(_cursor, GetAttr(0), count);
	_cursor += count;

	Invalidate(Rect(0, GetCursorPos().y-1, _canvas.Size().x, 2));
}

void
Console::PutTab()
{
	const int count = _tab - _canvas.GetOffset(_cursor)%_tab;
	if (_cursor+count > _canvas.BufferEnd()-1) {
		Scroll();
	}
	for(int i=0; i<count; i++) {
		*_cursor++ = GetAttr(0);
	}
	Invalidate(Rect(0, GetCursorPos().y-1, _canvas.Size().x, 2));
}

void
Console::PutReturn ()
{
	_cursor -= (((size_t)_cursor-(size_t)_canvas.Buffer())/2)%_canvas.Size().x;
}

void
Console::PutBackspace ()
{
	if ((char)(*(_cursor-1) & 0xFF) == '\0') {
		while((char)(*(--_cursor) & 0xFF) == '\0') {
		}
		_cursor++;
	} else {
		*(--_cursor) = GetAttr(0);
		const Vec2d p = GetCursorPos();
		Invalidate(Rect(p.x, p.y, 1, 1));
	}
}

void
Console::PutChar (char c)
{
	if (_cursor < _canvas.BufferEnd()-1) {
		const Vec2d p = GetCursorPos();
		*(_cursor++) = GetAttr(c);
		Invalidate(Rect(p.x, p.y, 1, 1));
	} else {
		*(_cursor++) = GetAttr(c);
		Scroll();
	}
}

void
Console::PutString (const char* s)
{
	if (s != NULL) {
		while (char c = *s++) {
			PutChar(c);
		}
	}
}

void
Console::PrintChar (char c)
{
	if (AnsiEscFilter(c)) {
		UpdateCursor();
		return;
	}
	switch(c) {
		case '\n': PutNewLine(); break;
		case '\t': PutTab(); break;
		case '\r': PutReturn(); break;
		case '\b': PutBackspace(); break;
		default: PutChar(c); break;
	}
	UpdateCursor();
}

Console&
Console::operator << (int32_t value)
{
	uint32_t ud;
	if (value < 0) {
		PutChar('-');
		ud = -value;
	} else {
		ud = value;
	}
	char buf[48];
	char* p = buf;
	do {
		*p++ = symb[ud % 10];
	} while (ud /= 10);

	while (p > buf) {
		PutChar(*--p);
	}

	return *this;
}

Console&
Console::operator << (uint32_t ud)
{
	char buf[48];
	char* p = buf;
	do {
		*p++ = symb[ud % 10];
	} while (ud /= 10);

	while (p > buf) {
		PutChar(*--p);
	}

	return *this;
}

Console&
Console::operator << (int64_t value)
{
	uint64_t ud;
	if (value < 0) {
		PutChar('-');
		ud = -value;
	} else {
		ud = value;
	}
	char buf[48];
	char* p = buf;
	do {
		*p++ = symb[ud % 10];
	} while (ud /= 10);

	while (p > buf) {
		PutChar(*--p);
	}

	return *this;
}

Console&
Console::operator << (uint64_t ud)
{
	char buf[48];
	char* p = buf;
	do {
		*p++ = symb[ud % 10];
	} while (ud /= 10);

	while (p > buf) {
		PutChar(*--p);
	}

	return *this;
}
/*
Console&
Console::operator << (const ptr_t i)
{
	size_t ud = (size_t)i;
	const int len = sizeof(size_t)*2;
	char buf[len+1];
	char* p = &buf[len];
	*p-- = '\0';
	do {
		*p-- = symb[ud % 16];
	} while (ud /= 16);

	while (p >= buf) {
		*p-- = '0';
	}
	PutString("0x");
	PutString(buf);
	return *this;
}
*/
Console&
Console::operator << (bool b)
{
	PutString(b ? "true" : "false");
	return *this;
}

Console&
Console::operator << (const char* s)
{
	if (!s)
		s = "<null>";
	while (char c = *s++)
		PrintChar(c);
	return *this;
}

void
Console::Redraw (const Rect& region)
{
	argused(region);
}

void
Console::Scroll()
{
	memmove(_canvas.Buffer(), _canvas.Buffer()+_canvas.Size().x, (_canvas.Size().x*(_canvas.Size().y-1))*sizeof(uint16_t));
	_cursor -= _canvas.Size().x;
	for(int i=0; i<_canvas.Size().x; i++) {
		_cursor[i] = GetAttr(0);
	}
	Invalidate();
}
