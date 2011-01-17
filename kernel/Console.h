#ifndef _INC_CONSOLE_H
#define _INC_CONSOLE_H

#include <std/Stack.h>
#include "CliWindow.h"

extern const char endl;
extern const char esc;

/// Simplifies access to text screen buffer
class Console
	: public CliWindow
{

public:
	/// Constructor 
	Console(const Rect& bounds);

	/// Destructor
	virtual ~Console();

	/// Prints a character
	Console& operator <<(char c);

	/// Prints an integer
	Console& operator <<(int i);

	/// Prints an unsigned integer
	Console& operator <<(size_t i);

	/// Prints a memory address (in hex)
	Console& operator <<(const void* i);

	/// Prints a boolean (true or false)
	Console& operator <<(bool b);

	/// Prints an ansi string
	Console& operator <<(const char* s);

	/// Resets all attributes to defaults
	void ResetAttributes();
	/// Sets line-wrapping
	void SetLineWrap(bool enabled);
	/// Sets reverse video mode
	void SetReverseVideo(bool enabled);
	/// Sets blink mode
	void SetBlink(bool enabled);
	/// Sets bold mode
	void SetBold(bool enabled);

private:
	///
	enum EraseDirection {
		FORWARD, BACKWARD, BOTH
	};

	/// Clears the screen with background color
	void Erase(EraseDirection d, bool line);

	/// Scrolls 1 line
	void Scroll();

	/// Puts any character as it is
	void PutChar(char c);
	/// Puts any string as it is
	void PutString(const char* s);
	/// Puts a new line
	void PutNewLine();
	/// Puts a tab
	void PutTab();
	/// Puts a caret return
	void PutReturn();
	/// Puts a backspace
	void PutBackspace();

	/// Prints a character, detecting it's type and filtering escape sequences
	void PrintChar(const char c);

	/// Overloaded redraw method
	void Redraw(const Rect& region);

	/// Filters ansi escape sequences and executes requested commands
	bool AnsiEscFilter(const char c);

	/// @returns Current cursor position
	Vec2d GetCursorPos();

	/// Updates cursor to current position
	void UpdateCursor();

private:

	uint16_t*	_cursor;	/// Pointer to current cursor location
	int			_tab;		/// Tab width
	bool		_lineWrap;	/// Line wrapping switch
	bool		_reverseVideo;	/// Reverse video switch


	char		_ansiBuffer[30];	/// Buffer for current esc sequence
	int			_ansiBufferPos;		/// Position in sequence buffer
	Stack<uint16_t*, 64> _ansiCursorStack;		/// Saved cursor positions
};

inline
Console&
Console::operator <<(const char c)
{
	PrintChar(c);
	return *this;
}

inline
Console::~Console()
{
}

#endif //_INC_CONSOLE_H

