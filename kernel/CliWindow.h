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
	Vec2d mOffset;
	Vec2d mClientOffset;
	CliWindow* mParent;
	Color mBackground;
	Color mForeground;
	String mCaption;

	uint16_t GetAttr(int c) const {
		return static_cast<uint16_t>(((mBackground << 4 | mForeground) << 8) | (c & 0xFF));
	}


public:
	Canvas<uint16_t> mCanvas;

	CliWindow(const Rect& bounds);
	virtual ~CliWindow();

	virtual void Redraw(const Rect& region);

	virtual void SetCursor(const Vec2d& p);

	void Invalidate(const Rect& region);
	void Invalidate();

	void Move(const Vec2d& p);
	void Resize(int w, int h);

	Color Background() const { return mBackground; }
	void Background(Color val) { mBackground = val; OnBackgroundChanged(); }
	Color Foreground() const { return mForeground; }
	void Foreground(Color val) { mForeground = val; OnForegroundChanged(); }
	const String& Caption() const { return mCaption; }
	void Caption(String val) { mCaption = val; OnCaptionChanged(); }

	CliWindow* Parent() { return mParent; }
	void Parent(CliWindow* val) { mParent = val; }
	const Vec2d& ClientOffset() const { return mClientOffset; }
	void ClientOffset(const Vec2d& val) { mClientOffset = val; }

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

	const Rect Bounds() const { return Rect(mOffset.x, mOffset.y, mCanvas.Size().x, mCanvas.Size().y); }

};

