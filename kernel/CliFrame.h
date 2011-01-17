#pragma once
#include "CliWindow.h"

/// Frame Console Window
class CliFrame
	: public CliWindow
{

public:
	/// Constructor
	CliFrame(const Rect& bounds, CliWindow* client)
		: CliWindow(bounds)
		, _client(client)
	{
		_client->Parent(this);
		ClientOffset(Vec2d(1, 1));
	}

	/// Destructor
	~CliFrame() {
	}

	/// Overloaded
	void SetCursor (const Vec2d& p) {
		Rect r2 = _canvas.GetDrawBounds();
		r2.Width(r2.Width() - 2);
		r2.Height(r2.Height() - 2);
		r2.Left(r2.Height() + 1);
		r2.Top(r2.Top() + 1);
		if (r2.Contains(p)) {
			if (_parent) {
				_parent->SetCursor(p + _offset + _parent->ClientOffset());
			}
		} else {
			_parent->SetCursor(Vec2d(-1,-1));
		}
	}

	/// Overloaded
	void Redraw(const Rect& region) {
		CliWindow::Redraw(region);

		Rect b = _client->Bounds();
		b.Move(ClientOffset());
		if (b.Intersects(region)) {
			Rect bc(b.Intersect(region));
			bc.Move(Vec2d(-b.Left(), -b.Top()));
			_canvas.Copy(_client->_canvas, bc, bc.TopLeft()+b.TopLeft());
		}

		for(int i=1; i<_canvas.Size().x-1; i++) {
			*_canvas.OffsetOf(i,0) = GetAttr(205);
			*_canvas.OffsetOf(i,_canvas.Size().y-1) = GetAttr(205);
		}
		for(int i=1; i<_canvas.Size().y-1; i++) {
			*_canvas.OffsetOf(0,i) = GetAttr(186);
			*_canvas.OffsetOf(_canvas.Size().x-1,i) = GetAttr(186);
		}
		*_canvas.OffsetOf(0, 0) = GetAttr(201);
		*_canvas.OffsetOf(0, _canvas.Size().y-1) = GetAttr(200);
		*_canvas.OffsetOf(_canvas.Size().x-1, 0) = GetAttr(187);
		*_canvas.OffsetOf(_canvas.Size().x-1, _canvas.Size().y-1) = GetAttr(188);

		const char* capt = _caption;
		uint16_t* cdst = _canvas.OffsetOf(2, 0);
		*cdst++ = GetAttr(' ');
		while(*capt) {
			*cdst++ = GetAttr(*capt++);
		}
		*cdst++ = GetAttr(' ');

		*_canvas.OffsetOf(_canvas.Size().x-2, 0) = GetAttr('X');
		*_canvas.OffsetOf(_canvas.Size().x-3, 0) = GetAttr('.');
	}

private:
	CliWindow* _client;		/// Content
};

