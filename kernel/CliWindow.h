#pragma once

#include <std/Vec2d.h>
#include <std/Rect.h>
#include <std/List.h>
#include <std/String.h>
#include "Canvas.h"

class CliWindow
{

public:
	enum Color{
		BLACK, BLUE,   GREEN, CYAN,   RED,   MAGENTA,   BROWN,  WHITE,
		GRAY,  BLUE_B, LIME,  CYAN_B, RED_B, MAGENTA_B, YELLOW, WHITE_B
	};

protected:
	Vec2d		_offset;
	Vec2d		_clientOffset;
	CliWindow*	_parent;
	Color		_background;
	Color		_foreground;
	String		_caption;

	uint16_t GetAttr(int c) const {
		return static_cast<uint16_t>(((_background << 4 | _foreground) << 8) | (c & 0xFF));
	}


public:
	Canvas<uint16_t> _canvas;

	CliWindow(const Rect& bounds);
	virtual ~CliWindow();

	virtual void Redraw(const Rect& region);

	virtual void SetCursor(const Vec2d& p);

	void Invalidate(const Rect& region);
	void Invalidate();

	void Move(const Vec2d& p);
	void Resize(int w, int h);

	Color Background() const { return _background; }
	void Background(Color val) { _background = val; OnBackgroundChanged(); }
	Color Foreground() const { return _foreground; }
	void Foreground(Color val) { _foreground = val; OnForegroundChanged(); }
	const String& Caption() const { return _caption; }
	void Caption(String val) { _caption = val; OnCaptionChanged(); }

	CliWindow* Parent() { return _parent; }
	void Parent(CliWindow* val) { _parent = val; }
	const Vec2d& ClientOffset() const { return _clientOffset; }
	void ClientOffset(const Vec2d& val) { _clientOffset = val; }

public:
	virtual void OnBackgroundChanged() {
		Invalidate();
	};
	virtual void OnForegroundChanged() {
		Invalidate();
	};
	virtual void OnCaptionChanged() {
		Invalidate();
	};

	const Rect Bounds() const { return Rect(_offset.x, _offset.y, _canvas.Size().x, _canvas.Size().y); }

};

